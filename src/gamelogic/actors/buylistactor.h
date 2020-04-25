#pragma once

#include "actor.h"
#include "turn.h"
#include "reaction.h"
#include "actor_helpers.h"
#include "buylist.h"

#include "cards.h"

#include <iostream>

#define info(x) x

class BuylistActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;
    void react(EventReactOptions& options) override;

    BuylistCollection buylists;
};

void BuylistActor::react(EventReactOptions& options)
{
    for (auto& option: options) {
        defaultReact(*option);
    }
}

void BuylistActor::executeTurn(Turn* turn)
{
    info(std::cerr << "*** Turn" << turn->turnCount() << std::endl);

    defaultVillageDraw(turn, 0);

    auto hand = turn->currentHand();
    for (auto& card: hand.cards) {
        info(std::cerr << "  Card " << card.card->name() << std::endl);
        defaultPlay(turn, card);
    }

    hand = turn->currentHand();
    playAllTreasures(hand);

    buylists.buy(turn);
}

