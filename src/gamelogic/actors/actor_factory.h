#pragma once

#include "bigmoneyactor.h"
#include "engineactor.h"
#include "shepherdactor.h"
#include "buylistactor.h"
#include "buylist/parser.h"

#include "defaultbuylist.h"
#include "defaultcondition.h"

#include <string>
#include <memory>

inline std::unique_ptr<Actor> createActor(std::string const& which, Supply const* supply, Deck const* deck, std::any args) {
    if (which == "bigmoney")
        return std::make_unique<BigMoneyActor>(supply, deck);

    if (which == "engine")
        return std::make_unique<EngineActor>(supply, deck);

    if (which == "shepherd")
        return std::make_unique<ShepherdActor>(supply, deck);

    if (which == "buylist") {
        auto actor = std::make_unique<BuylistActor>(supply, deck);
        auto argsPair = std::any_cast<std::pair<BuylistCollection, StrategyCollection>>(args);
        actor->buylists = argsPair.first;
        actor->strats = argsPair.second;
        return actor;
    }

    throw SetupError{"No such actor"};
}
