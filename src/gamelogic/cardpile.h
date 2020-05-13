#pragma once

#include "card.h"

struct CardMoveError {
    std::string reason;
};

class CardPile {
public:
    CardPile() = default;
    CardPile(Cards const& cards);

    void reserve(int n);
    bool empty() const;

    void put(Card* card);
    void put(Cards cards);

    void moveCardTo(int index, CardPile& target);
    void moveCardTo(Card* card, CardPile& target);
    void moveAllTo(CardPile& target);

    int count() const;
    int count(CardId id) const;
    int count(Card::Type type) const;
    Cards& cards();
    Cards const& cards() const;
    Card* topCard();

    Cards findCards(CardId id) const;
    bool hasCard(CardId id) const;

    Card* draw();
    Cards drawWithShuffle(int n, CardPile& reserve);

    void shuffle();

private:
    Cards m_cards;
};
