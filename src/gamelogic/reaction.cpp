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
    if (name == "Haggler"s)
        return ReactKind::HagglerReaction;
    if (name == "Masquerade"s)
        return ReactKind::MasqueradeReaction;

    if (name == "TorturerAttack"s)
        return ReactKind::TorturerAttack;
    if (name == "MilitiaAttack"s)
        return ReactKind::MilitiaAttack;
    if (name == "SwindlerAttack"s)
        return ReactKind::SwindlerAttack;
    if (name == "MinionAttack"s)
        return ReactKind::MinionAttack;

    return ReactKind::Invalid;
}

GainAttackReactOption::GainAttackReactOption(Deck* deck, CardId gain, int count)
    : AttackReactOption(ReactKind::NoChoiceAttack, deck)
    , m_gain(gain)
    , m_count(count)
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

void discardDownTo(Deck* deck, int n, const DiscardFunc& prefer)
{
    for (auto* card: prefer(deck->constHand().cards())) {
        if (deck->constHand().count() <= n) {
            break;
        }
        deck->discardFromHand(card);
    }

    // If we didn't get enough cards to discard, enforce by just discarding whatever.
    while (deck->constHand().count() > n) {
        deck->discardFromHand(deck->constHand().cards().back());
    }
}

