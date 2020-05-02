#pragma once

#include "actor.h"
#include "turn.h"
#include "reaction.h"
#include "actor_helpers.h"
#include "defaultplay.h"
#include "genericplay.h"
#include "buylist.h"
#include "strategy.h"

#include "cards.h"

#include <iostream>

#define info(x)

class BuylistActor: public Actor {
public:
    using Actor::Actor;
    void executeTurn(Turn* turn) override;
    void react(EventReactOptions& options) override;

    BuylistCollection buylists;
    StrategyCollection strats;
};
