#pragma once

#include "cardpile.h"
#include "deck.h"
#include "supply.h"

struct Hand;
struct ActiveCard;
class Turn;

enum class TurnPhase {
    Action,
    PlayTreasures,
    Buy,
    Cleanup,
    DrawNext,
    Ended
};

struct InvalidPlayError {
    std::string error;
};

/// Internal state of a turn. This is hidden from the actor, but provided
/// to the card executors because they need to modify it.
struct TurnInternal {
    Turn* turn;

    CardPile cardsInPlay;
    TurnPhase phase = TurnPhase::Action;

    Supply* supply;
    Deck* deck;

    int actions = 1;
    int buys = 1;
    int money = 0;
};

/// The Turn state machine. Tracks turn state and allows to perform actions
/// with your current hand.
class Turn {
public:
    Turn(Supply* supply, Deck* deck);

    Hand currentHand();
    std::vector<ActiveCard> cardsInPlay() const;
    TurnPhase currentPhase();
    void endTurn();

    int currentMoney() const;

    Cards doFinalDraw();

    template<typename CardT> bool buy();

private:
    friend struct ActiveCard;
    void playAction(Card* card, CardOption* option);
    void playTreasure(Card* card, CardOption* option);

    Deck* deck();

    TurnInternal m_internal;
};

/// Convenience wrapper which puts together a card and a turn.
struct ActiveCard {
    Turn* turn;
    Card* card;

    bool inHand;

    void playAction(CardOption* option = nullptr) {
        turn->playAction(card, option);
    }

    void playTreasure(CardOption* option = nullptr) {
        turn->playTreasure(card, option);
    }
};

/// Convenience wrapper which represents cards in your hand.
struct Hand {
    std::vector<ActiveCard> cards;

    std::vector<ActiveCard> treasureCards() const;
};

template<typename CardT> bool Turn::buy()
{
    if (currentPhase() > TurnPhase::Buy) {
        throw InvalidPlayError{"You already ended your buy phase."};
    }
    m_internal.phase = TurnPhase::Buy;

    auto* pile = m_internal.supply->pile<CardT>();
    if (pile->empty()) {
        return false;
    }

    pile->moveCardTo(0, m_internal.deck->discardPile());
}
