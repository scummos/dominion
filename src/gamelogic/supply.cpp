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
    createPile<Province>(4);
    createPile<Smithy>(10);
    createPile<Festival>(10);
    createPile<Chapel>(10);
    createPile<Shepherd>(10);
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

CardPile& Supply::trashPile()
{
    return m_trashPile;
}

CardPile& Supply::pile(const CardId id)
{
    auto pile = std::find_if(m_piles.begin(), m_piles.end(), [id](auto const& pile) { return pile.kind == id; });
    if (pile == m_piles.end()) {
        throw NoSuchPileError{"The requested pile does not exist."};
    }
    return *pile;

}
