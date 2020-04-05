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
    Supply();
    ~Supply();

    CardPile* pile(CardId const id);
    CardPile* discardPile();

    int countEmptyPiles() const;

protected:
    template<typename T>
    void createPile(int count);

private:
    std::vector<SupplyCardPile> m_piles;
    CardPile m_discardPile;
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
    m_piles.emplace_back(std::move(pile));
}
