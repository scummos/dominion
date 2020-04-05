#pragma once

#include "actor.h"
#include "turn.h"

#include "cards.h"

#include <iostream>
#define info(x)

class ShepherdActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;

    int shepherd = 0;
    int turnCount = 0;
};

void ShepherdActor::executeTurn(Turn* turn)
{
    turnCount++;
    auto const totalCards = turn->currentTotalCards();

    auto hand = turn->currentHand();

    while (!turn->currentHand().findCards(CardId::Shepherd).empty()) {
        auto shepherd = turn->currentHand().findCards(CardId::Shepherd).front();
        CardOptionShepherd opt;
        for (auto const& victory: turn->currentHand().findCards(Card::Victory)) {
            opt.discard.push_back(victory.card);
        }
        for (auto* discard: opt.discard) {
            info(std::cerr << "    discard " << discard->name() << std::endl);
        }
        shepherd.playAction(&opt);
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
    else if (turn->currentMoney() >= 4 && shepherd <= 1) {
        turn->buy(CardId::Shepherd);
        shepherd++;
    }
    else if (turn->currentMoney() >= 3) {
        turn->buy(CardId::Silver);
    }
    else if (turn->currentMoney() >= 2) {
        turn->buy(CardId::Estate);
    }
}
