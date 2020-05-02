#pragma once

#include "buylist.h"
#include "strategy.h"

#include <utility>

std::pair<BuylistCollection, StrategyCollection> parseBuylist(char const* filename);
