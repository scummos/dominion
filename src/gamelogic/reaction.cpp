#include "reaction.h"
#include "event.h"
#include "deck.h"

#include <iostream>

ReactKind reactKind(std::string const& name)
{
    using std::operator""s;

    if (name == "IgnoreAttack"s)
        return ReactKind::IgnoreAttackReaction;
    if (name == "Trader"s)
        return ReactKind::TraderReaction;

    if (name == "DiscardAttack"s)
        return ReactKind::DiscardAttack;
    if (name == "TorturerAttack"s)
        return ReactKind::TorturerAttack;

    return ReactKind::Invalid;
}

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
    m_deck->gainFromSupply(CardId::Curse);
}

int GainAttackReactOption::count() const
{
    return m_count;
}

CardId GainAttackReactOption::gain() const
{
    return m_gain;
}

EventReactOption::EventReactOption(ReactKind kind)
    : m_kind(kind)
{
}

void IgnoreAttackReactOption::accept()
{
    event.ignored = true;
}

IgnoreAttackReactOption::IgnoreAttackReactOption(Event& event)
    : EventReactOption(ReactKind::IgnoreAttackReaction)
    , event(event)
{
}
