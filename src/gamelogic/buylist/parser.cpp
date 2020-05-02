#include "parser.h"
#include "strategy.h"
#include "buylist.tab.hpp"

extern int parse_buylist(char const* fn, BuylistCollection& ret, StrategyCollection& strat);

std::pair<BuylistCollection, StrategyCollection> parseBuylist(char const* fn) {
    BuylistCollection ret;
    StrategyCollection strat;
    parse_buylist(fn, ret, strat);
    return {ret, strat};
}
