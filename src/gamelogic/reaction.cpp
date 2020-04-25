#include "reaction.h"
#include "event.h"

GainAttackReactOption::GainAttackReactOption(Deck* deck, CardId gain, int count)
    : AttackReactOption(ReactKind::NoChoiceAttack, deck)
    , m_gain(gain)
    , m_count(count)
{
}

void DiscardAttackReactOption::defaultAction()
{
}

void GainAttackReactOption::defaultAction()
{
}

int GainAttackReactOption::count() const
{
    return m_count;
}

CardId GainAttackReactOption::gain() const
{
    return m_gain;
}

EventReactOption::EventReactOption(ReactKind kind, Deck* deck)
    : m_kind(kind)
    , m_deck(deck)
{
}

void IgnoreAttackReactOption::accept()
{
    event.ignored = true;
}

IgnoreAttackReactOption::IgnoreAttackReactOption(Deck* deck, Event& event)
    : EventReactOption(ReactKind::IgnoreAttackReaction, deck)
    , event(event)
{
}
