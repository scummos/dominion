#include "supply.h"

#include "cards.h"

#include <algorithm>

Supply::Supply()
{
    createPile<Copper>(30);
    createPile<Silver>(30);
    createPile<Gold>(30);
    createPile<Estate>(11);
    createPile<Duchy>(8);
    createPile<Province>(8);
}

Supply::~Supply()
{
    for (auto& pile: m_piles) {
        std::for_each(pile.cards().begin(), pile.cards().end(), [](Card* c) { delete c; });
    }
}

int Supply::countEmptyPiles() const {
    int ret = 0;
    for (auto& pile: m_piles) {
        ret += pile.empty();
    }
    return ret;
}
