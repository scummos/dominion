#include "logger.h"

#include "game.h"
#include "deck.h"

#include <math.h>

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
