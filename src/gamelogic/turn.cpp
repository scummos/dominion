#include "turn.h"

#include <algorithm>

Turn::Turn(Supply* supply, Deck* deck)
{
    m_internal.turn = this;
    m_internal.supply = supply;
    m_internal.deck = deck;
}

ActiveCards Hand::treasureCards() const
{
    ActiveCards ret;
    for (auto const& c: cards) {
        if (c.card->hasType(Card::Treasure)) {
            ret.push_back(c);
        }
    }
    return ret;
}

ActiveCards Hand::findCards(CardId id) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card.card->basicInfo().id == id) {
            ret.push_back(card);
        }
    }
    return ret;
}

ActiveCards Hand::findCards(Card::Hints hints) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card.card->hints() & hints) {
            ret.push_back(card);
        }
    }
    return ret;
}

bool Hand::hasCard(CardId id) const
{
    return std::any_of(cards.begin(), cards.end(), [id](ActiveCard const& c) {
        return c.card->basicInfo().id == id;
    });
}

void Turn::buy(CardId id)
{
    if (currentPhase() > TurnPhase::Buy) {
        throw InvalidPlayError{"You already ended your buy phase."};
    }
    m_internal.phase = TurnPhase::Buy;

    auto* pile = m_internal.supply->pile(id);
    if (pile->empty()) {
        throw InvalidPlayError{"This supply pile is empty."};
    }

    if (m_internal.buys <= 0) {
        throw InvalidCardUsage{"You have no buys."};
    }
    m_internal.buys--;

    pile->moveCardTo(0, *m_internal.deck->discardPile());
}

Hand Turn::currentHand()
{
    auto cards = m_internal.deck->currentHand()->cards();

    Hand ret;
    ret.cards.reserve(cards.size());
    for (auto* card: cards) {
        ret.cards.push_back(ActiveCard{this, card});
    }
    return ret;
}

int Turn::currentMoney() const
{
    return m_internal.money;
}

TurnPhase Turn::currentPhase()
{
    return m_internal.phase;
}

Deck* Turn::deck()
{
    return m_internal.deck;
}

int Turn::currentTotalCards() const
{
    return m_internal.deck->totalCards();
}

int Turn::currentActions() const
{
    return m_internal.actions;
}

void Turn::playAction(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::Action) {
        throw InvalidPlayError{"This turn is not in its action phase."};
    }
    m_internal.phase = TurnPhase::Action;

    if (m_internal.actions <= 0) {
        throw InvalidPlayError{"You have no actions left."};
    }
    m_internal.actions--;

    card->playAction(&m_internal, option);
    deck()->currentHand()->moveCardTo(card, m_internal.cardsInPlay);
}

void Turn::playTreasure(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::PlayTreasures) {
        throw InvalidPlayError{"You already finished playing treasures and cannot play any more."};
    }
    m_internal.phase = TurnPhase::PlayTreasures;

    card->playTreasure(&m_internal, option);
    deck()->currentHand()->moveCardTo(card, m_internal.cardsInPlay);
}

void Turn::endTurn()
{
    if (currentPhase() > TurnPhase::Cleanup) {
        throw InvalidPlayError{"This turn already ended."};
    }
    m_internal.phase = TurnPhase::Cleanup;

    m_internal.cardsInPlay.moveAllTo(*deck()->discardPile());
    deck()->currentHand()->moveAllTo(*deck()->discardPile());
}

Cards Turn::doFinalDraw()
{
    if (currentPhase() > TurnPhase::DrawNext) {
        throw InvalidPlayError{"Attempting to draw next hand twice"};
    }
    m_internal.phase = TurnPhase::Ended;

    return m_internal.deck->drawCards(5);
}

void TurnInternal::draw(int n)
{
    deck->currentHand()->put(deck->drawCards(n));
}

void TurnInternal::trashFromHand(Card* card)
{
    deck->currentHand()->moveCardTo(card, *supply->discardPile());
}

