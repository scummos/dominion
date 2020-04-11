#include "game.h"
#include "logger.h"

#include <iostream>
#include <fstream>

int main() {
    int const games = 10000;
    Logger::instance(games);

    std::vector<int> winners(2);
    for (int i = 0; i < games; i++) {
        Game game({"bigmoney", "bigmoney"});
        game.run();
    }

    auto const& money = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalMoney);
    auto const& score = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalScore);
    auto const& cards = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TotalCards);
//     auto const& peakMoney = Logger::instance()->computeTurnGraph(0, PerTurnLogData::TurnPeakMoney);

    auto writeData = [](auto const& d, std::string fn) {
        std::ofstream fd;
        fd.open(fn);
        for (auto& point: d) {
            fd << point.value << "\t" << point.error << "\n";
        }
        fd.close();
    };

    writeData(money, "turnTotalMoney.data");
    writeData(score, "turnTotalScore.data");
    writeData(cards, "turnTotalCards.data");
}
