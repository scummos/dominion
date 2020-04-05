#pragma once

#include "actor.h"
#include "turn.h"
#include "actor_helpers.h"

#include "cards.h"

#include <iostream>

#define info(x)

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
    info(std::cerr << "** Turn " << turnCount << std::endl);

    defaultVillageDraw(turn, 0);

    auto hand = turn->currentHand();
    for (auto& card: hand.cards) {
        info(std::cerr << "  Card " << card.card->name() << std::endl);
    }
    auto treasure = plainTreasureInHand(hand);
    if (turnCount == 1 || turnCount == 2) {
        if (chapels == 0 && treasure != 5) {
            turn->buy(CardId::Chapel);
            info(std::cerr << "    Buy Chapel" << std::endl);
            chapels++;
        }
        else {
            turn->buy(CardId::Silver);
            info(std::cerr << "    Buy Silver" << std::endl);
        }
        return;
    }

    auto chapels = hand.findCards(CardId::Chapel);
    if (!chapels.empty() && (treasure < 6 || (treasure < 8 && turnCount > 8))) {
        CardOptionChapel opt;
        for (auto const& estate: hand.findCards(CardId::Estate)) {
            opt.trash.push_back(estate.card);
        }

        auto coppers = hand.findCards(CardId::Copper);
        if (hand.findCards(CardId::Silver).size() == 1 && !coppers.empty()) {
            coppers.pop_back();
        }
        for (auto const& copper: coppers) {
            opt.trash.push_back(copper.card);
        }
        for (auto* trash: opt.trash) {
            info(std::cerr << "    Trash " << trash->name() << std::endl);
        }
        chapels.front().playAction(&opt);
    }

    for (auto& treasure: turn->currentHand().treasureCards()) {
        treasure.playTreasure();
    }
    if (turn->currentMoney() >= 8) {
        turn->buy(CardId::Province);
        info(std::cerr << "    Buy Province" << std::endl);
    }
    else if (turn->currentMoney() >= 6) {
        turn->buy(CardId::Gold);
        info(std::cerr << "    Buy Gold" << std::endl);
    }
    else if (turn->currentMoney() >= 3) {
        turn->buy(CardId::Silver);
        info(std::cerr << "    Buy Silver" << std::endl);
        silvers++;
    }
}
