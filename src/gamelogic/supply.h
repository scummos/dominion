#pragma once

#include "cardpile.h"

#include <optional>
#include <typeindex>

struct NoSuchPileError {
    std::string error;
};

class SupplyCardPile : public CardPile {
public:
    using CardPile::CardPile;
    CardId kind;
};

class Supply {
public:
    Supply(int nplayers);
    ~Supply();

    CardPile& pile(CardId const id);
    Card::BasicInfo pileInfo(CardId const id);
    CardPile& trashPile();

    int countEmptyPiles() const;

protected:
    template<typename T>
    void createPile(int count);

private:
    std::vector<SupplyCardPile> m_piles;
    std::vector<Card::BasicInfo> m_pileInfos;
    CardPile m_trashPile;
};

// template implementations
template<typename T> void Supply::createPile(int count)
{
    static_assert(sizeof(T) == sizeof(Card), "invalid Card type");

    SupplyCardPile pile;
    pile.kind = T().basicInfo().id;
    for (int i = 0; i < count; i++) {
        pile.put(new T);
    }
    m_pileInfos.emplace_back(pile.topCard()->basicInfo());
    m_piles.emplace_back(std::move(pile));
}
