#pragma once

#include "cardpile.h"

// This class simultaneously represents a player.
class Deck {
public:
    Deck(std::vector<Card*> startingCards);

    CardPile* drawPile();
    CardPile* discardPile();
    CardPile* currentHand();
    CardPile const* drawPile() const;
    CardPile const* discardPile() const;
    CardPile const* currentHand() const;

    Cards drawCards(int n);
    int countScoreInDrawPile() const;
    int totalCards() const;

private:
    CardPile m_drawPile;
    CardPile m_discardPile;
    CardPile m_currentHand;
};
