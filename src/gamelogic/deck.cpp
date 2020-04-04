#include "deck.h"

Deck::Deck(std::vector<Card*> startingCards)
{
    for (auto* card: startingCards) {
        m_drawPile.put(card);
    }
    m_drawPile.shuffle();
}

CardPile* Deck::discardPile()
{
    return &m_discardPile;
}

CardPile* Deck::drawPile()
{
    return &m_drawPile;
}

const CardPile* Deck::discardPile() const
{
    return &m_discardPile;
}

const CardPile* Deck::drawPile() const
{
    return &m_drawPile;
}

CardPile * Deck::currentHand()
{
    return &m_currentHand;
}

const CardPile * Deck::currentHand() const
{
    return &m_currentHand;
}

Cards Deck::drawCards(int n)
{
    return m_drawPile.drawWithShuffle(n, m_discardPile);
}

int Deck::countScoreInDrawPile() const
{
    int score = 0;

    for (auto const* card: m_drawPile.cards()) {
        score += card->victoryPoints();
    }

    return score;
}
