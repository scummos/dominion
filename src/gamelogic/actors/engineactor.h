#pragma once

#include "actor.h"
#include "turn.h"
#include "actor_helpers.h"

#include "cards.h"

#include <iostream>

class EngineActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;

    int smithies = 0;
    int festivals = 0;
    int chapels = 0;
    int turnCount = 0;
    int silvers = 0;
};

void EngineActor::executeTurn(Turn* turn)
{
    turnCount++;

    defaultVillageDraw(turn, 0);

    auto hand = turn->currentHand();
    auto treasure = plainTreasureInHand(hand);
    if (turnCount == 1 || turnCount == 2) {
        if (chapels == 0 && treasure != 5) {
            turn->buy(CardId::Chapel);
        }
        else {
            turn->buy(CardId::Silver);
        }
        return;
    }

    auto chapels = hand.findCards(CardId::Chapel);
    if (!chapels.empty()) {
        CardOptionChapel opt;
        for (auto& estate: hand.findCards(CardId::Estate)) {
            opt.trash.push_back(estate.card);
        }
        if (silvers > 1) {
            for (auto& copper: hand.findCards(CardId::Copper)) {
                opt.trash.push_back(copper.card);
            }
        }
        chapels.front().playAction(&opt);
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
    else if (turn->currentMoney() >= 3) {
        turn->buy(CardId::Silver);
        silvers++;
    }
}
