#pragma once

#include "buylist.h"

// Some buylists convenience helpers.

std::vector<BuyAction> anyVictory() {
    return {CardId::Province, CardId::Duchy, CardId::Estate};
};

std::vector<BuyAction> goodVictory() {
    return {CardId::Province, CardId::Duchy, CardId::Estate};
};

std::vector<BuyAction> bestMoney() {
    return {CardId::Gold, CardId::Silver};
};

