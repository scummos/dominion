#include "game.h"
#include "logger.h"

#include "buylist/parser.h"

#include <iostream>
#include <fstream>
#include <numeric>
#include <execution>
#include <sstream>

namespace {
    using std::operator""s;

    struct IOError {
        std::string message;
    };

    std::string readFile(char const* fn) {
        std::cout << "Reading file " << fn << std::endl;
        std::ifstream stream(fn);
        if (!stream) {
            throw IOError{"Failed to open file: "s + std::string(fn)};
        }
        std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: dominion strategyfile1 strategyfile2" << std::endl;
        exit(2);
    }

    int const games = 25000;
    auto logger = Logger::instance(games);

    std::any args1, args2;
    try {
        args1 = parseBuylist(readFile(argv[1]).data());
        args2 = parseBuylist(readFile(argv[2]).data());
    }
    catch (IOError e) {
        std::cerr << "error loading input files: " << e.message << std::endl;
        exit(1);
    }

    std::vector<int> winners(2);
    int draws = 0;

    std::vector<int> gameNumber(games);
    std::iota(gameNumber.begin(), gameNumber.end(), 0);
    std::mutex winners_mutex;

    std::for_each(std::execution::par_unseq, gameNumber.begin(), gameNumber.end(), [&](int) {
        Game game({"buylist", "buylist"}, {args1, args2});
        auto winner = game.run();

        logger->addGame(game.logData());
        std::lock_guard lock(winners_mutex);
        if (winner >= 0) {
            winners[winner]++;
        }
        else {
            draws++;
        }
    });

    std::cout << "Player 1 wins " << winners.at(0) << " out of " << games << " games, " << draws << " draws" << std::endl;
    std::cout << "(winrate " << (double) winners.at(0)/games*100 << "% vs " << (double) winners.at(1)/games*100 << "%)" << std::endl;

    auto writeData = [](auto const& d, std::string fn) {
        std::ofstream fd;
        fd.open(fn);
        for (auto& point: d) {
            fd << point.value << "\t" << point.error << "\n";
        }
        fd.close();
    };

    auto const& money = logger->computeTurnGraph(0, PerTurnLogData::TotalMoney);
    auto const& score = logger->computeTurnGraph(0, PerTurnLogData::TotalScore);
    auto const& cards = logger->computeTurnGraph(0, PerTurnLogData::TotalCards);
    auto const& pkMoney = logger->computeTurnGraph(0, PerTurnLogData::TurnPeakMoney);
    auto const& nCurses = logger->computeTurnGraph(1, PerTurnLogData::Curses);

    writeData(money, "turnTotalMoney.data");
    writeData(score, "turnTotalScore.data");
    writeData(cards, "turnTotalCards.data");
    writeData(pkMoney, "turnPeakMoney.data");
    writeData(nCurses, "turnTotalCurses.data");

    auto writeHist = [](auto const& d, auto const& dims, std::string fn) {
        std::ofstream fd;
        fd.open(fn);
        fd << "# ";
        for (auto const& dim: dims) {
            fd << static_cast<int>(dim.dim) << "," << dim.bins << "," << dim.lower << "," << dim.upper << " ";
        }
        fd << "\n";

        for (auto const point: d.data()) {
            fd << point << "\n";
        }
        fd.close();
    };

    auto dims = {
        HistogramDimension{
            PerTurnLogData::TurnNumber,
            40,
            0, 39
        },
        HistogramDimension{
            PerTurnLogData::TurnPeakMoney,
            20,
            0, 19
        }
    };
    auto hist = logger->computeHistogramNd(0, dims);

    writeHist(hist, dims, "turnMoneyPeak.hist");
}
