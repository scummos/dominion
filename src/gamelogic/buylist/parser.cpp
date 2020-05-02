#include "parser.h"
#include "strategy.h"
#include "buylist.tab.hpp"

extern int parse_buylist(char const* text, BuylistCollection& ret, StrategyCollection& strat);

std::pair<BuylistCollection, StrategyCollection> parseBuylist(char* text) {
    BuylistCollection ret;
    StrategyCollection strat;
    parse_buylist(text, ret, strat);
    return {ret, strat};
}
