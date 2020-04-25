#pragma once

#include "reaction.h"

class Card;
struct AttackReactOption;

enum class EventKind {
    Attack,
    CardDiscardedFromHand,
    EnemyGainsACard,
    YouGainACard,
};

/**
 * An Event describes something happening in the game. These objects are never
 * visible to the Game, Deck or Actor; they are exclusively used to query Card instances
 * for possible Reactions (usually done by the Turn). Everything should, if possible, operate
 * on the Reaction instances, since they can check the game's rules (i.e. the Card sets them
 * up such that nothing wrong can be done with them).
 */
struct Event {
    Event(EventKind kind);

    EventKind kind;
    bool ignored = false;
};

struct AttackEvent : public Event {
    AttackEvent(EventReactOption::Ptr attack);

    EventReactOption::Ptr attack;
};

struct YouGainACardEvent : public Event {
    YouGainACardEvent(Card* card) : Event(EventKind::YouGainACard), card(card) {};

    Card* card;
};

struct EnemyGainsACardEvent : public Event {
    EnemyGainsACardEvent(Card* card) : Event(EventKind::EnemyGainsACard), card(card) {};

    Card* card;
};

struct CardDiscardedFromHandEvent : public Event {
    CardDiscardedFromHandEvent(Card* card) : Event(EventKind::CardDiscardedFromHand), card(card) {}

    Card* card;
};
