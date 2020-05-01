#pragma once

#include "actor.h"
#include "turn.h"
#include "reaction.h"
#include "actor_helpers.h"
#include "defaultplay.h"
#include "buylist.h"

#include "cards.h"

#include <iostream>

#define info(x)

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
    info(std::cerr << "*** Turn " << turn->turnCount() << std::endl);

    defaultVillageDraw(turn, 0);

    auto hand = turn->currentHand();
    auto didPlaySometing = true;
    while (didPlaySometing) {
        didPlaySometing = false;
        info(std::cerr << " --- " << std::endl);
        for (auto& card: hand.cards) {
            info(std::cerr << "  Card " << card.card->name() << std::endl);
            didPlaySometing = defaultPlay(turn, card);
            if (didPlaySometing) {
                // Might be that our hand changed
                break;
            }
        }
        hand = turn->currentHand();
    }

    hand = turn->currentHand();
    while (hand.hasCard(Card::Treasure)) {
        playAllTreasures(turn, hand);
        hand = turn->currentHand();
    }

    buylists.buy(turn);
}

