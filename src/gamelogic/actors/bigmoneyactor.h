#pragma once

#include "actor.h"
#include "turn.h"

#include "cards.h"

class BigMoneyActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;
};

void BigMoneyActor::executeTurn(Turn* turn)
{
    auto hand = turn->currentHand();
    for (auto& treasure: hand.treasureCards()) {
        treasure.playTreasure();
    }

    switch (turn->currentMoney()) {
        case 15: case 14: case 13: case 12: case 11: case 10: case 9: case 8:
            turn->buy<Province>();
            break;

        case 7: case 6:
            turn->buy<Gold>();
            break;

        case 5: case 4: case 3:
            turn->buy<Silver>();
            break;

        default:
            break;
    }
}
