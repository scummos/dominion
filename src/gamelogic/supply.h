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
    bool filled = false;
    int initialCount = 10;
};

class Supply {
public:
    Supply(int nplayers);
    ~Supply();

    CardPile& pile(CardId const id);
    Card::BasicInfo pileInfo(CardId const id) const;
    CardPile& trashPile();
    CardId getAnyWithCost(Cost const& cost) const;

    int countEmptyPiles() const;

protected:
    void fillPile(SupplyCardPile& p);
    Card* createCard(CardId id);
    void createPile(CardId id, int count);

private:
    std::vector<SupplyCardPile> m_piles;
    std::vector<Card::BasicInfo> m_pileInfos;
    CardPile m_trashPile;
};

// template implementations
