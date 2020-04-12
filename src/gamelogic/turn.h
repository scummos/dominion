#pragma once

#include "cardpile.h"
#include "deck.h"
#include "supply.h"

#include <iostream>

struct Hand;
struct ActiveCard;
class Turn;

// using ActiveCards = QVarLengthArray<ActiveCard, 16>;
using ActiveCards = std::vector<ActiveCard>;

enum class TurnPhase {
    Action,
    PlayTreasures,
    Buy,
    Cleanup,
    DrawNext,
    Ended
};

/// Internal state of a turn. This is hidden from the actor, but provided
/// to the card executors because they need to modify it.
struct TurnInternal {
    Turn* turn;

    TurnPhase phase = TurnPhase::Action;

    Supply* supply;
    Deck* deck;

    void addActions(int actions) {
        m_actions += actions;
        m_maxActions = std::max(m_actions, m_maxActions);
    }
    void addBuys(int buys) {
        m_buys += buys;
        m_maxBuys = std::max(m_buys, m_maxBuys);
    }
    void addMoney(int money) {
        m_money += money;
        m_maxMoney = std::max(m_money, m_maxMoney);
    }

    int actions() const {
        return m_actions;
    }
    int buys() const {
        return m_buys;
    }
    int money() const {
        return m_money;
    }

    // Some useful primitives used to implement cards.
    int draw(int n);
    void trashFromHand(Card* card);
    void discardFromHand(Card* card);

private:
    int m_actions = 1;
    int m_buys = 1;
    int m_money = 0;

    int m_maxActions = 0;
    int m_maxBuys = 0;
    int m_maxMoney = 0;
    int m_totalCardsSeen = 0;

    friend class Turn;
};

/// The Turn state machine. Tracks turn state and allows to perform actions
/// with your current hand.
class Turn {
public:
    Turn(Supply* supply, Deck* deck);

    Hand currentHand();
    ActiveCards cardsInPlay() const;
    TurnPhase currentPhase();
    int turnCount();
    void endTurn();

    int currentMoney() const;
    int currentTotalCards() const;
    int currentActions() const;

    int leftInSupply(CardId id) const;

    int doFinalDraw();

    void buy(CardId id);

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
    ActiveCards cards;

    ActiveCards treasureCards() const;
    ActiveCards findCards(CardId id) const;
    ActiveCards findCards(Card::Type type) const;
    ActiveCards findCards(Card::Hints hints) const;
    bool hasCard(CardId id) const;
};

