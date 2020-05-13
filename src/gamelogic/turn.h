#pragma once

#include "cardpile.h"
#include "deck.h"
#include "supply.h"
#include "event.h"
#include "reaction.h"
#include "logger.h"
#include "game.h"

#include <iostream>
#include <map>

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
    void addDiscount(int discount) {
        m_discount += discount;
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

    Cost cardCost(CardId id) const;

    // Some useful primitives used to implement cards.
    int draw(int n);
    void trashFromHand(Card* card);
    void discardFromHand(Card* card);
    int countCardsInHand() const;
    int countCardsInHand(Card::Type type) const;
    void attackEachEnemy(AttackReactOption::Factory attack);
    int numPlayed(CardId card) const;
    int numPlayed(Card::Type type) const;

private:
    int m_actions = 1;
    int m_buys = 1;
    int m_money = 0;
    int m_discount = 0;

    int m_maxActions = 0;
    int m_maxBuys = 0;
    int m_maxMoney = 0;
    int m_totalCardsSeen = 0;

    // TODO this map causes a lot of memory allocations, maybe better have a vector or a stack array?
    std::map<CardId, int> m_numPlayed;

    friend class Turn;
};

/// The Turn state machine. Tracks turn state and allows to perform actions
/// with your current hand.
class Turn {
public:
    Turn(Supply* supply, Deck* deck, Logger::PlayerData& logData, Game::LogFunction func = Game::LogFunction());

    Hand currentHand();
    Cards currentHandCards();
    int currentHandSize() const;
    ActiveCards cardsInPlay() const;
    TurnPhase currentPhase();
    int turnCount();
    void endTurn();

    int currentMoney() const;
    int currentTotalCards() const;
    int currentActions() const;
    int currentBuys() const;

    int leftInSupply(CardId id) const;
    int emptySupplyPiles() const;
    int totalCards(CardId id) const;
    Cost cardCost(CardId id) const;

    int doFinalDraw();

    void buy(CardId id);

    bool loggingEnabled() const;
    void log(std::string message);

private:
    friend struct ActiveCard;
    friend class Condition;
    void playAction(Card* card, CardOption* option);
    void playTreasure(Card* card, CardOption* option);

    Deck* deck();

    TurnInternal m_internal;
    Logger::PlayerData& m_logData;
    Game::LogFunction m_logFunc;
};

/// Convenience wrapper which puts together a card and a turn.
struct ActiveCard {
    Turn* turn;
    Card* card;

    void playAction(CardOption* option = nullptr) {
        turn->playAction(card, option);
    }

    void playTreasure(CardOption* option = nullptr) {
        turn->playTreasure(card, option);
    }
};

/// Convenience wrapper which represents cards in your hand.
struct Hand {
    Hand(Cards const& cards, Turn* turn);

    Cards const& cards;

    ActiveCards activeCards() const;
    ActiveCard activeCard(Card* card) const;

    ActiveCards treasureCards() const;
    ActiveCards findCards(CardId id) const;
    ActiveCards findCards(Card::Type type) const;
    ActiveCards findCards(Card::Hints hints) const;
    bool hasCard(CardId id) const;
    bool hasCard(Card::Type type) const;
    void ignore(Card* card);

private:
    Turn* turn;
    Card* ignored = nullptr;
};

