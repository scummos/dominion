#pragma once

#include "cardid.h"
#include "card.h"

#include <optional>
#include <vector>
#include <memory>

class Deck;

enum class ReactKind {
    IgnoreAttackReaction, //< Option to ignore an attack (e.g. Moat)
    GainDifferentCardReaction, //< Option to gain a different card instead
    Attack = 0x1000,
    DiscardAttack = 2 | Attack, //< You are forced to discard some cards
    TorturerAttack = 3 | Attack, //< Special attack by torturer card
    NoChoiceAttack = 4 | Attack, //< Generic attack type which does something but gives you no choice
};

/**
 * Describes how an entity, usually a Card or an Actor, can react to some event. If a choice is possible, the structure
 * should provide an interface to let the Actor choose what to do. If the reaction is
 * mandatory, i.e. the player cannot chose to ignore it, override defaultAction(); it will be called
 * after the Actor was given the structure to do something with.
 *
 * Instances of these structures are always obtained from the card classes, but in two different possible ways:
 *  a) either, a card can react to an event (see event.h) or
 *  b) a card can trigger an Attack. The Attack itself is described by what other players
 *     can (or have to) do about it. Attacks are routed through the turn, which first checks
 *     if they trigger any other cards (see option a), and then passes them to the actor.
 *
 * In the a) case, the ReactOption is returned by the Card's reactToEvent method, which iscalled by the Deck's
 *   eventOccured method, then passed to the Actor to do something with.
 * In the b) case, the ReactOption is instantiated by the card when the card is played (e.g. playAction) and
 *   delivered to the affected deck(s) (e.g. via the Turn's attackEachEnemy method).
 */
struct EventReactOption {
    using Ptr = std::shared_ptr<EventReactOption>;
    EventReactOption(ReactKind kind, Deck* deck);

    /// This should e.g. discard 2 random cards for a "discard 2 cards" attack *if* the
    /// actor did not chose which ones to discard.
    virtual void defaultAction() {};
    ReactKind kind() const { return m_kind; }

protected:
    ReactKind m_kind;
    Deck* m_deck;
};

using EventReactOptions = std::vector<EventReactOption::Ptr>;

/// Base type for attacks. These represent the attack itself, as well as possible options for it.
struct AttackReactOption : public EventReactOption {
protected:
    AttackReactOption(ReactKind kind, Deck* deck) : EventReactOption(kind, deck) {};
};

/// Forces the player to discard down to N, or M cards from his hand, but lets him
/// chose which ones.
struct DiscardAttackReactOption : public AttackReactOption {
    DiscardAttackReactOption(Deck* deck);

    void accept(std::vector<Card*> const& discard);
    void defaultAction() override;

private:
    std::optional<int> m_downto;
    std::optional<int> m_count;

    bool m_done = false;
};

/// Forces the player to gain a card. No options.
struct GainAttackReactOption : public AttackReactOption {
    GainAttackReactOption(Deck* deck, CardId gain, int count);

    virtual void defaultAction() override;

    CardId gain() const;
    int count() const;

private:
    CardId m_gain;
    int m_count;

    bool m_done = false;
};

/// Gives the Actor the option to ignore the attack (currently only created by Moat).
struct IgnoreAttackReactOption : public EventReactOption {
    IgnoreAttackReactOption(Deck* deck, Event& event);
    void accept();

private:
    Event& event;
};

