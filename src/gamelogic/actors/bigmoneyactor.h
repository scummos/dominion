#pragma once

#include "actor.h"
#include "turn.h"

#include "cards.h"

#include <iostream>

class BigMoneyActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;

    int smithies = 0;
    int festivals = 0;
    int turnCount = 0;
};

void BigMoneyActor::executeTurn(Turn* turn)
{
    turnCount++;
    auto const totalCards = turn->currentTotalCards();

    auto hand = turn->currentHand();

    for (auto& festival: hand.findCards(CardId::Festival)) {
        festival.playAction();
    }

    for (auto& smithy: hand.findCards(CardId::Smithy)) {
        if (turn->currentActions() <= 0) {
            break;
        }
        smithy.playAction();
    }

    for (auto& treasure: turn->currentHand().treasureCards()) {
        treasure.playTreasure();
    }

    if (turn->currentMoney() >= 8) {
        turn->buy(CardId::Province);
    }
    else if (turn->currentMoney() >= 6) {
        turn->buy(CardId::Gold);
    }
//     else if (turn->currentMoney() >= 5 && festivals < 10) {
//         turn->buy(CardId::Festival);
//         festivals++;
//     }
    else if (turn->currentMoney() >= 4 && smithies <= 2) {
        turn->buy(CardId::Smithy);
        smithies++;
    }
    else if (turn->currentMoney() >= 3) {
        turn->buy(CardId::Silver);
    }

#if 0
    switch (turn->currentMoney()) {
        case 15: case 14: case 13: case 12: case 11: case 10: case 9: case 8:
            if (totalCards > 15) {
                turn->buy<Province>();
                break;
            }
            [[fallthrough]];
        case 7: case 6:
            turn->buy<Gold>();
            break;

        case 5: case 4:
            if (smithies == 0 || (smithies == 1 && totalCards > 14)) {
                turn->buy<Smithy>();
                smithies++;
                break;
            }
            [[fallthrough]];
        case 3:
            if (turn->currentMoney() == 5) {
                turn->buy<Festival>();
                break;
            }
            turn->buy<Silver>();
            break;

        default:
            break;
    }
#endif
}
