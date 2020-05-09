#pragma once

#include "buylistactor.h"
#include "buylist/parser.h"

#include "defaultbuylist.h"
#include "defaultcondition.h"

#include <string>
#include <memory>

inline std::unique_ptr<Actor> createActor(std::string const& which, Supply const* supply, Deck const* deck, std::any args) {
    if (which == "buylist") {
        auto actor = std::make_unique<BuylistActor>(supply, deck);
        auto argsPair = std::any_cast<std::pair<BuylistCollection, StrategyCollection>>(args);
        actor->buylists = argsPair.first;
        actor->strats = argsPair.second;
        return actor;
    }

    throw SetupError{"No such actor"};
}
