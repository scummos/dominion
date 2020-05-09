#pragma once

#include "supply.h"
#include "cardpile.h"
#include "deck.h"
#include "event.h"
#include "reaction.h"

class Turn;

class Actor {
public:
    Actor(Supply const* supply, Deck const* deck);
    virtual ~Actor() = default;

    Supply const* supply() const;
    Deck const* deck() const;
    CardPile const& drawPile() const;
    CardPile const& discardPile() const;

    /// Invoked when the actor needs to play a turn. Implement your turn logic here.
    virtual void executeTurn(Turn* turn) = 0;

    /// Invoked when the actor can react to some event. The @p options will often have only one
    /// entry, but there might be multiple things you can do to react to e.g. an attack, in which
    /// case you can choose the order in which they happen.
    virtual void react(EventReactOptions& options);

protected:
    /// The supply of the game. You cannot modify or use it, but inspect its state.
    Supply const* m_supply;
    /// Your deck. You cannot modify it, but inspect its state.
    Deck const* m_deck;
};
