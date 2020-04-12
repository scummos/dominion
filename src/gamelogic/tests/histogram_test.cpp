#include "logger.h"

#include <iostream>

int main() {
    auto l = Logger::instance();

    // Sample game 1
    l->nextGame();
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);

    l->addData(0, PerTurnLogData::TotalMoney, 3);
    l->addData(0, PerTurnLogData::TotalMoney, 4);
    l->addData(0, PerTurnLogData::TotalMoney, 5);
    l->addData(0, PerTurnLogData::TotalMoney, 5);
    l->addData(0, PerTurnLogData::TotalMoney, 5);

    l->addData(0, PerTurnLogData::TurnNumber, 0);
    l->addData(0, PerTurnLogData::TurnNumber, 1);
    l->addData(0, PerTurnLogData::TurnNumber, 2);
    l->addData(0, PerTurnLogData::TurnNumber, 3);
    l->addData(0, PerTurnLogData::TurnNumber, 4);

    // Sample game 2
    l->nextGame();
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 5);
    l->addData(0, PerTurnLogData::CardsSeen, 7);
    l->addData(0, PerTurnLogData::CardsSeen, 7);

    l->addData(0, PerTurnLogData::TotalMoney, 3);
    l->addData(0, PerTurnLogData::TotalMoney, 4);
    l->addData(0, PerTurnLogData::TotalMoney, 5);
    l->addData(0, PerTurnLogData::TotalMoney, 5);
    l->addData(0, PerTurnLogData::TotalMoney, 5);

    l->addData(0, PerTurnLogData::TurnNumber, 0);
    l->addData(0, PerTurnLogData::TurnNumber, 1);
    l->addData(0, PerTurnLogData::TurnNumber, 2);
    l->addData(0, PerTurnLogData::TurnNumber, 3);
    l->addData(0, PerTurnLogData::TurnNumber, 4);

    auto hist = l->computeHistogramNd(0, {
        HistogramDimension{
            PerTurnLogData::TurnNumber,
            10,
            0, 10
        },
        HistogramDimension{
            PerTurnLogData::CardsSeen,
            10,
            0, 10
        },
    });

    // turn 0, 5 cards seen
    std::cerr << hist.value({0, 5}) << std::endl;
}
