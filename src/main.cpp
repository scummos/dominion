#include "game.h"
#include "logger.h"

#include "buylist/parser.h"

#include <iostream>
#include <fstream>

int main() {
    int const games = 5000;
    Logger::instance(games);

    std::vector<int> winners(2);
    int draws = 0;
    for (int i = 0; i < games; i++) {
        Game game({"buylist", "buylistBM"});
        auto winner = game.run();
        if (winner >= 0) {
            winners[winner]++;
        }
        else {
            draws++;
        }
    }

    std::cout << "Player 1 wins " << winners.at(0) << " out of " << games << " games, " << draws << " draws" << std::endl;
    std::cout << "(winrate " << (float) winners.at(0)*100/games << "% vs " << (float) winners.at(1)*100/games << "%)" << std::endl;

    auto writeData = [](auto const& d, std::string fn) {
        std::ofstream fd;
        fd.open(fn);
        for (auto& point: d) {
            fd << point.value << "\t" << point.error << "\n";
        }
        fd.close();
    };

    auto const& money = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalMoney);
    auto const& score = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalScore);
    auto const& cards = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalCards);
    auto const& pkMoney = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TurnPeakMoney);
//     auto const& peakMoney = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TurnPeakMoney);

    writeData(money, "turnTotalMoney.data");
    writeData(score, "turnTotalScore.data");
    writeData(cards, "turnTotalCards.data");
    writeData(pkMoney, "turnPeakMoney.data");

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
            PerTurnLogData::CardsSeen,
            20,
            0, 19
        }
    };
    auto hist = Logger::instance()->computeHistogramNd(0, dims);

    writeHist(hist, dims, "turnMoneyPeak.hist");
}
