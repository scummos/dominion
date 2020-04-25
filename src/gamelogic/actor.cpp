#include "actor.h"

Actor::Actor(Supply const* supply, Deck const* deck)
    : m_supply(supply)
    , m_deck(deck)
{
}

CardPile const& Actor::discardPile() const
{
    return m_deck->discardPile();
}

CardPile const& Actor::drawPile() const
{
    return m_deck->drawPile();
}

Deck const* Actor::deck() const
{
    return m_deck;
}

Supply const* Actor::supply() const
{
    return m_supply;
}

void Actor::react(EventReactOptions& /*options*/)
{
    // Default actor does nothing.
}
