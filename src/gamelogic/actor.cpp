#include "actor.h"

Actor::Actor(Supply const* supply, Deck const* deck)
    : m_supply(supply)
    , m_deck(deck)
{
}

const CardPile* Actor::discardPile() const
{
    return m_deck->discardPile();
}

const CardPile* Actor::drawPile() const
{
    return m_deck->drawPile();
}

const Deck* Actor::deck() const
{
    return m_deck;
}

const Supply* Actor::supply() const
{
    return m_supply;
}
