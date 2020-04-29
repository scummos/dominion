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

inline std::unique_ptr<Actor> createActor(std::string const& which, Supply const* supply, Deck const* deck) {
    static auto disk_buylist = parseBuylist("../src/gamelogic/buylist/test2.buylist");

    if (which == "bigmoney")
        return std::make_unique<BigMoneyActor>(supply, deck);

    if (which == "engine")
        return std::make_unique<EngineActor>(supply, deck);

    if (which == "shepherd")
        return std::make_unique<ShepherdActor>(supply, deck);

    if (which == "buylistBM") {
        auto actor = std::make_unique<BuylistActor>(supply, deck);
        actor->buylists = BuylistCollection{{
            Buylist {
                If<VeryLateGame>(), anyVictory()
            },
            Buylist {
                If<LateGame>(), goodVictory()
            },
            Buylist {
                If<CardCountGreaterThan>(CardId::Gold, 0), {CardId::Province}
            },
            Buylist {{
                {
                    If<AnyOf>(
                        If<CardCountLessThan>(CardId::Smithy, 1),
                        If<AllOf>(
                            If<CardCountLessThan>(CardId::Smithy, 2),
                            If<CardCountGreaterThan>(CardId::Gold, 0)
                        )
                    ),
                    CardId::Smithy
                },
                {
                    If<AllOf>(
                        If<CardCountLessThan>(CardId::Moat, 1),
                        IfNot<HasMoney>(3)
                    ),
                    CardId::Moat
                }
            }},
            Buylist {
                bestMoney()
            },
        }};
        return actor;
    }

    if (which == "buylist") {
        auto actor = std::make_unique<BuylistActor>(supply, deck);
        actor->buylists = disk_buylist;
        return actor;
    }

    throw SetupError{"No such actor"};
}
