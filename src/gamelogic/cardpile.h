#pragma once

#include "card.h"

struct CardMoveError {
    std::string reason;
};

class CardPile {
public:
    CardPile() = default;
    CardPile(Cards cards);

    bool empty() const;

    void put(Card* card);
    void put(Cards cards);

    void moveCardTo(int index, CardPile& target);
    void moveAllTo(CardPile& target);

    Cards& cards();
    Cards const& cards() const;
    Card* topCard();

    Card* draw();
    Cards drawWithShuffle(int n, CardPile& reserve);

    void shuffle();

private:
    Cards m_cards;
};
