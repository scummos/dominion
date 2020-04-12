#include "logger.h"

#include "game.h"
#include "deck.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <iostream>

HistogramNd::HistogramNd(std::vector<HistogramDimension> const& dims)
    : m_dims(dims)
{
    auto size = std::accumulate(dims.begin(), dims.end(), 1, [](int k, auto const& dim) {
        return k * dim.bins;
    });
    m_data.resize(size, 0);
}

std::vector<int> HistogramNd::data() const
{
    return m_data;
}

int& HistogramNd::value(MultiIndex const& index)
{
    int mul = 1;
    int offset = 0;
    int n = 0;
    for (auto const& rowOffset: index) {
        offset += mul * rowOffset;
        mul *= m_dims.at(n).bins;
        n++;
    }
    return m_data.at(offset);
}

int HistogramNd::valueToRowIndex(PerTurnLogData dim, double value)
{
    auto dimInfo = std::find_if(m_dims.begin(), m_dims.end(), [dim](const auto& histoDim) {
        return histoDim.dim == dim;
    });
    if (dimInfo == m_dims.end()) {
        throw InvalidIndexError{"No such dimension."};
    }

    if (value <= dimInfo->lower) {
        return 0;
    }
    if (value >= dimInfo->upper) {
        return dimInfo->bins - 1;
    }
    auto const span = dimInfo->upper - dimInfo->lower;
    auto const ret = static_cast<int> (((value - dimInfo->lower) * dimInfo->bins) / span);
    return ret;
}

HistogramNd Logger::computeHistogramNd(int playerIndex, std::vector<HistogramDimension> const& dims)
{
    HistogramNd ret(dims);
    MultiIndex multiIndex(dims.size());

    for (auto const& game: m_games) {
        auto const& vdata = game[playerIndex].m_vectorData;
        for (int i = 0; i < vdata[0].size(); i++) {
            for (int axis = 0; axis < dims.size(); axis++) {
                auto const dim = dims[axis].dim;
                auto const value = vdata[static_cast<int>(dim)][i];
                multiIndex[axis] = ret.valueToRowIndex(dim, value);
            }
            ret.value(multiIndex)++;
        }
    }

    return ret;
}

Logger::PlayerData::PlayerData()
{
    for (int i = 0; i < static_cast<int>(PerTurnLogData::Num); i++) {
        m_vectorData[i].reserve(32);
    }
}

Logger::Logger(int preallocGames)
{
    m_games.reserve(preallocGames);
}

Logger* Logger::instance(int preallocGames)
{
    static Logger logger(preallocGames);
    return &logger;
}

void Logger::nextGame()
{
    m_games.emplace_back();
}

Logger::GamePlayerData& Logger::currentGame()
{
    return m_games.back();
}

Logger::PlayerData& Logger::currentPlayerData(int index)
{
    if (currentGame().size() <= index) {
        currentGame().resize(index+1);
    }
    return currentGame().at(index);
}

void Logger::addData(int playerIndex, PerTurnLogData which, int value)
{
    auto& playerData = currentPlayerData(playerIndex);
    playerData.m_vectorData[static_cast<int>(which)].push_back(value);
}

void Logger::addData(int playerIndex, PerGameLogData which, int value)
{
    auto& playerData = currentPlayerData(playerIndex);
    playerData.m_scalarData[static_cast<int>(which)] = value;
}

std::vector<DataPoint> Logger::computeTurnGraph(int playerIndex, PerTurnLogData which)
{
    // number of data points used per turn
    std::vector<int> n;
    // sum of values per turn
    std::vector<int64_t> sum;
    // sum of squares of values per turn
    std::vector<int64_t> sumSq;

    auto ensureResized = [&sum, &sumSq, &n](int maxIndex) {
        auto const size = maxIndex+1;
        if (n.size() >= size) {
            return;
        }
        n.resize(size, 0);
        sum.resize(size, 0.0);
        sumSq.resize(size, 0.0);
    };

    auto forEachTurn = [&](auto func) {
        for (auto const& game: m_games) {
            auto const& playerData = game.at(playerIndex);
            auto const& data = playerData.m_vectorData[static_cast<int>(which)];
            for (int i = 0; i < data.size(); i++) {
                ensureResized(i);
                func(data[i], i);
            }
        }
    };

    // Accumulate results to compute averages.
    forEachTurn([&](auto const& turnData, int turn) {
        n[turn]++;
        sum[turn] += turnData;
    });

    // Assemble return vector, without standard deviations.
    std::vector<DataPoint> ret;
    for (int i = 0; i < n.size(); i++) {
        auto const avg = sum[i] / (double) n[i];
        ret.push_back({
            avg,
            0
        });
    }

    // Compute standard deviations.
    forEachTurn([&](auto const& turnData, int turn) {
        auto const& dev = turnData - ret[turn].value;
        sumSq[turn] += dev*dev;
    });

    // Add standard deviations to result vector.
    for (int i = 0; i < n.size(); i++) {
        if (n.at(i) <= 1) {
            ret[i].error = std::numeric_limits<double>::quiet_NaN();
            continue;
        }
        ret[i].error = std::sqrt(1./(n[i]-1) * sumSq[i]);
    }

    return ret;
}

#if 0

    PlayerData const* playerData = nullptr;

    std::function<void(int)> recurseDims = [&](int dimIndex) {
        auto const dim = dims[dimIndex].dim;
        auto const& vdata = playerData->m_vectorData[static_cast<int>(dim)];

        if (dimIndex == dims.size()) {
            // Innermost loop, this is where the histogram is actually computed.
            std::cerr << "visit: ";
            for (auto const i: multiIndex) {
                std::cerr << i << " ";
            }
            std::cerr << std::endl;
            ret.value(multiIndex)++;
        }

        // Otherwise, recurse further.
        for (int i = dimIndex; i < dims.size(); i++) {
            for (int j = 0; j < vdata.size(); j++) {
                std::cerr << "  recurse: dim " << i << " index " << j << std::endl;
                multiIndex[i] = ret.valueToRowIndex(dims[i].dim, vdata[j]);
                recurseDims(i + 1);
            }
        }
    };

    for (auto const& game: m_games) {
        playerData = &game.at(playerIndex);
        recurseDims(0);
    }

#endif
