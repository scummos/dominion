#pragma once

#include "supply.h"
#include "cardpile.h"
#include "deck.h"

class Turn;

class Actor {
public:
    Actor(Supply const* supply, Deck const* deck);
    virtual ~Actor() = default;

    Supply const* supply() const;
    Deck const* deck() const;
    CardPile const& drawPile() const;
    CardPile const& discardPile() const;

    virtual void executeTurn(Turn* turn) = 0;

protected:
    Supply const* m_supply;
    Deck const* m_deck;
};
