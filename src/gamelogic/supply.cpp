#include "supply.h"

#include "cards.h"

#include <algorithm>

Supply::Supply(int nplayers)
{
    auto nVictoryCards = nplayers == 1 ? 4 : nplayers == 2 ? 8 : 12;

    createPile<Copper>(100);
    createPile<Silver>(100);
    createPile<Gold>(100);
    createPile<Estate>(nVictoryCards + 3*nplayers);
    createPile<Duchy>(nVictoryCards);
    createPile<Province>(nVictoryCards);
    createPile<Smithy>(10);
    createPile<Festival>(10);
    createPile<Chapel>(10);
    createPile<Shepherd>(10);
    createPile<Curse>(10);
    createPile<Witch>(10);
    createPile<Mine>(10);
    createPile<Moat>(10);
    createPile<Moneylender>(10);
    createPile<Torturer>(10);
    createPile<CouncilRoom>(10);
    createPile<Mandarin>(10);
    createPile<FoolsGold>(10);
    createPile<Rebuild>(10);
    createPile<Remodel>(10);
    createPile<Oasis>(10);
    createPile<Highway>(10);
    createPile<Laboratory>(10);
    createPile<IllGottenGains>(10);
    createPile<Embassy>(10);
    createPile<Trader>(10);
    createPile<Cartographer>(10);
    createPile<Crossroads>(10);
    createPile<Margrave>(10);
    createPile<Militia>(10);
    createPile<Haggler>(10);
    createPile<Nobles>(10);
}

Supply::~Supply()
{
    for (auto& pile: m_piles) {
        std::for_each(pile.cards().begin(), pile.cards().end(), [](Card* c) { delete c; });
    }
    std::for_each(m_trashPile.cards().begin(), m_trashPile.cards().end(), [](Card* c) { delete c; });
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

Card::BasicInfo Supply::pileInfo(const CardId id) const
{
    auto info = std::find_if(m_pileInfos.begin(), m_pileInfos.end(), [id](auto const& info) { return info.id == id; });
    if (info == m_pileInfos.end()) {
        throw NoSuchPileError{"The requested pile does not exist."};
    }
    return *info;
}

