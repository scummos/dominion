#include "logger.h"

#include <iostream>

int main() {
    auto l = Logger::instance();

    // Sample game 1
    Logger::GameData g1(1);
    g1[0].addData(PerTurnLogData::CardsSeen, 5);
    g1[0].addData(PerTurnLogData::CardsSeen, 5);
    g1[0].addData(PerTurnLogData::CardsSeen, 5);
    g1[0].addData(PerTurnLogData::CardsSeen, 5);
    g1[0].addData(PerTurnLogData::CardsSeen, 5);

    g1[0].addData(PerTurnLogData::TotalMoney, 3);
    g1[0].addData(PerTurnLogData::TotalMoney, 4);
    g1[0].addData(PerTurnLogData::TotalMoney, 5);
    g1[0].addData(PerTurnLogData::TotalMoney, 5);
    g1[0].addData(PerTurnLogData::TotalMoney, 5);

    g1[0].addData(PerTurnLogData::TurnNumber, 0);
    g1[0].addData(PerTurnLogData::TurnNumber, 1);
    g1[0].addData(PerTurnLogData::TurnNumber, 2);
    g1[0].addData(PerTurnLogData::TurnNumber, 3);
    g1[0].addData(PerTurnLogData::TurnNumber, 4);

    // Sample game 2
    Logger::GameData g2(1);
    g2[0].addData(PerTurnLogData::CardsSeen, 5);
    g2[0].addData(PerTurnLogData::CardsSeen, 5);
    g2[0].addData(PerTurnLogData::CardsSeen, 5);
    g2[0].addData(PerTurnLogData::CardsSeen, 7);
    g2[0].addData(PerTurnLogData::CardsSeen, 7);

    g2[0].addData(PerTurnLogData::TotalMoney, 3);
    g2[0].addData(PerTurnLogData::TotalMoney, 4);
    g2[0].addData(PerTurnLogData::TotalMoney, 5);
    g2[0].addData(PerTurnLogData::TotalMoney, 5);
    g2[0].addData(PerTurnLogData::TotalMoney, 5);

    g2[0].addData(PerTurnLogData::TurnNumber, 0);
    g2[0].addData(PerTurnLogData::TurnNumber, 1);
    g2[0].addData(PerTurnLogData::TurnNumber, 2);
    g2[0].addData(PerTurnLogData::TurnNumber, 3);
    g2[0].addData(PerTurnLogData::TurnNumber, 4);

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
