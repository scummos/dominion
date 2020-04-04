#include "turn.h"

Turn::Turn(Supply* supply, Deck* deck)
{
    m_internal.turn = this;
    m_internal.supply = supply;
    m_internal.deck = deck;
}

std::vector<ActiveCard> Hand::treasureCards() const
{
    std::vector<ActiveCard> ret;
    for (auto const& c: cards) {
        if (c.card->hasType(Card::Treasure)) {
            ret.push_back(c);
        }
    }
    return ret;
}

Hand Turn::currentHand()
{
    Hand ret;
    for (auto* card: m_internal.deck->currentHand()->cards()) {
        ret.cards.emplace_back(ActiveCard{this, card});
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

void Turn::playAction(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::Action) {
        throw InvalidPlayError{"This turn is not in its action phase."};
    }
    m_internal.phase = TurnPhase::Action;

    card->playAction(&m_internal, option);
}

void Turn::playTreasure(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::PlayTreasures) {
        throw InvalidPlayError{"You already finished playing treasures and cannot play any more."};
    }
    m_internal.phase = TurnPhase::PlayTreasures;

    card->playTreasure(&m_internal, option);
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

