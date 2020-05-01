#include "parser.h"
#include "buylist.tab.hpp"

extern int parse_buylist(char const* fn, BuylistCollection& ret);

BuylistCollection parseBuylist(char const* fn) {
    BuylistCollection ret;
    parse_buylist(fn, ret);
    return ret;
}
