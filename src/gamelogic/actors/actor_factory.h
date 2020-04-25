#include "bigmoneyactor.h"
#include "engineactor.h"
#include "shepherdactor.h"

#include <string>
#include <memory>

inline std::unique_ptr<Actor> createActor(std::string const& which, Supply const* supply, Deck const* deck) {
    if (which == "bigmoney")
        return std::make_unique<BigMoneyActor>(supply, deck);

    if (which == "engine")
        return std::make_unique<EngineActor>(supply, deck);

    if (which == "shepherd")
        return std::make_unique<ShepherdActor>(supply, deck);

    throw SetupError{"No such actor"};
}
