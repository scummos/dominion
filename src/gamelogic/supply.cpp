#include "supply.h"

#include "cards.h"

#include <algorithm>

void Supply::fillPile(SupplyCardPile& p)
{
    if (p.filled) {
        return;
    }
    p.reserve(p.initialCount);
    for (int i = 0; i < p.initialCount; i++) {
        p.put(createCard(p.kind));
    }
    p.filled = true;
}

void Supply::createPile(CardId id, int count)
{
    SupplyCardPile pile;
    pile.kind = id;
    pile.initialCount = count - 1;

    auto* sampleCard = createCard(id);
    pile.put(sampleCard);
    m_pileInfos.emplace_back(sampleCard->basicInfo());
    m_piles.emplace_back(std::move(pile));
}

Card* Supply::createCard(CardId id)
{
    switch (id) {
        case CardId::Baron:          return new Baron();
        case CardId::Bridge:         return new Bridge();
//         case CardId::Buerocrat:      return new Buerocrat();
        case CardId::Cartographer:   return new Cartographer();
//         case CardId::Cellar:         return new Cellar();
        case CardId::Chapel:         return new Chapel();
        case CardId::Conspirator:    return new Conspirator();
        case CardId::Copper:         return new Copper();
        case CardId::CouncilRoom:    return new CouncilRoom();
        case CardId::Courtyard:      return new Courtyard();
        case CardId::Crossroads:     return new Crossroads();
        case CardId::Curse:          return new Curse();
        case CardId::Duchy:          return new Duchy();
        case CardId::Duke:           return new Duke();
        case CardId::Embassy:        return new Embassy();
        case CardId::Estate:         return new Estate();
        case CardId::Festival:       return new Festival();
        case CardId::FoolsGold:      return new FoolsGold();
        case CardId::Gold:           return new Gold();
        case CardId::Haggler:        return new Haggler();
        case CardId::Harem:          return new Harem();
        case CardId::Highway:        return new Highway();
        case CardId::IllGottenGains: return new IllGottenGains();
        case CardId::Ironworks:      return new Ironworks();
        case CardId::Laboratory:     return new Laboratory();
        case CardId::Library:        return new Library();
        case CardId::Mandarin:       return new Mandarin();
        case CardId::Margrave:       return new Margrave();
        case CardId::Market:         return new Market();
        case CardId::Masquerade:     return new Masquerade();
        case CardId::Militia:        return new Militia();
        case CardId::Mine:           return new Mine();
        case CardId::MiningVillage:  return new MiningVillage();
        case CardId::Minion:         return new Minion();
        case CardId::Moat:           return new Moat();
        case CardId::Moneylender:    return new Moneylender();
        case CardId::Nobles:         return new Nobles();
        case CardId::Oasis:          return new Oasis();
        case CardId::Pawn:           return new Pawn();
        case CardId::Province:       return new Province();
        case CardId::Rebuild:        return new Rebuild();
        case CardId::Remodel:        return new Remodel();
        case CardId::ShantyTown:     return new ShantyTown();
        case CardId::Shepherd:       return new Shepherd();
        case CardId::Silver:         return new Silver();
        case CardId::Smithy:         return new Smithy();
        case CardId::Steward:        return new Steward();
        case CardId::Swindler:       return new Swindler();
        case CardId::ThroneRoom:     return new ThroneRoom();
        case CardId::Torturer:       return new Torturer();
        case CardId::Trader:         return new Trader();
        case CardId::TradingPost:    return new TradingPost();
        case CardId::Upgrade:        return new Upgrade();
        case CardId::Village:        return new Village();
//         case CardId::WishingWell:    return new WishingWell();
        case CardId::Witch:          return new Witch();
//         case CardId::Workshop:       createPile<Workshop>(); break;
        default: throw SetupError{"Invalid card requested (card not implemented?)"};
    }
}

Supply::Supply(int nplayers)
{
    m_piles.reserve(static_cast<int>(CardId::NumCards));

    auto nVictoryCards = nplayers == 1 ? 4 : nplayers == 2 ? 8 : 12;

    createPile(CardId::Copper, 60+7*nplayers);
    createPile(CardId::Silver, 40);
    createPile(CardId::Gold, 40);
    createPile(CardId::Estate, nVictoryCards + 3*nplayers);
    createPile(CardId::Duchy, nVictoryCards);
    createPile(CardId::Province, nVictoryCards);
    createPile(CardId::Smithy, 10);
    createPile(CardId::Festival, 10);
    createPile(CardId::Chapel, 10);
    createPile(CardId::Shepherd, 10);
    createPile(CardId::Curse, 10);
    createPile(CardId::Witch, 10);
    createPile(CardId::Mine, 10);
    createPile(CardId::Moat, 10);
    createPile(CardId::Moneylender, 10);
    createPile(CardId::Torturer, 10);
    createPile(CardId::CouncilRoom, 10);
    createPile(CardId::Mandarin, 10);
    createPile(CardId::FoolsGold, 10);
    createPile(CardId::Rebuild, 10);
    createPile(CardId::Remodel, 10);
    createPile(CardId::Oasis, 10);
    createPile(CardId::Highway, 10);
    createPile(CardId::Laboratory, 10);
    createPile(CardId::IllGottenGains, 10);
    createPile(CardId::Embassy, 10);
    createPile(CardId::Trader, 10);
    createPile(CardId::Cartographer, 10);
    createPile(CardId::Crossroads, 10);
    createPile(CardId::Margrave, 10);
    createPile(CardId::Militia, 10);
    createPile(CardId::Haggler, 10);
    createPile(CardId::Baron, 10);
    createPile(CardId::Bridge, 10);
    createPile(CardId::Conspirator, 10);
    createPile(CardId::Courtyard, 10);
    createPile(CardId::Duke, 10);
    createPile(CardId::Harem, 10);
    createPile(CardId::Ironworks, 10);
    createPile(CardId::Masquerade, 10);
    createPile(CardId::MiningVillage, 10);
    createPile(CardId::Minion, 10);
    createPile(CardId::Nobles, 10);
    createPile(CardId::Pawn, 10);
    createPile(CardId::ShantyTown, 10);
    createPile(CardId::Steward, 10);
    createPile(CardId::Swindler, 10);
    createPile(CardId::Torturer, 10);
    createPile(CardId::TradingPost, 10);
    createPile(CardId::Upgrade, 10);
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
    auto& p = static_cast<SupplyCardPile&>(*pile);
    if (!p.filled) {
        fillPile(p);
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

