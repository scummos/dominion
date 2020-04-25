#include "turn.h"

#include "error.h"
#include "logger.h"

#include <algorithm>
#include <utility>

Turn::Turn(Supply* supply, Deck* deck)
{
    m_internal.turn = this;
    m_internal.supply = supply;
    m_internal.deck = deck;
    m_internal.m_totalCardsSeen = static_cast<const Deck*>(deck)->hand().cards().size();
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

ActiveCards Hand::findCards(Card::Type type) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card.card->hasType(type)) {
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

    if (m_internal.buys() <= 0) {
        throw InvalidCardUsage{"You have no buys."};
    }

    auto const cost = m_internal.cardCost(id);
    if (!cost.canPay({currentMoney()})) {
        throw InvalidPlayError{"You don't have enough money to pay for this card."};
    }

    m_internal.addBuys(-1);
    m_internal.addMoney(-cost.gold());

    deck()->gainFromSupply(id);
}

Hand Turn::currentHand()
{
    auto const& cards = static_cast<const Deck*>(m_internal.deck)->hand().cards();

    Hand ret;
    ret.cards.reserve(cards.size());
    for (auto* card: cards) {
        ret.cards.push_back(ActiveCard{this, card});
    }
    return ret;
}

int Turn::currentMoney() const
{
    return m_internal.money();
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

int Turn::totalCards(CardId id) const
{
    return m_internal.deck->totalCards(id);
}

int Turn::leftInSupply(CardId id) const
{
    return m_internal.supply->pile(id).count();
}

int Turn::currentActions() const
{
    return m_internal.actions();
}

int Turn::currentBuys() const
{
    return m_internal.buys();
}

void Turn::playAction(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::Action) {
        throw InvalidPlayError{"This turn is not in its action phase."};
    }
    m_internal.phase = TurnPhase::Action;

    if (m_internal.actions() <= 0) {
        throw InvalidPlayError{"You have no actions left."};
    }
    m_internal.addActions(-1);
    deck()->moveCard(card, Areas::Hand, Areas::InPlay);

    card->playAction(&m_internal, option);
}

void Turn::playTreasure(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::PlayTreasures) {
        throw InvalidPlayError{"You already finished playing treasures and cannot play any more."};
    }
    m_internal.phase = TurnPhase::PlayTreasures;

    card->playTreasure(&m_internal, option);
    deck()->moveCard(card, Areas::Hand, Areas::InPlay);
}

void Turn::endTurn()
{
    if (currentPhase() > TurnPhase::Cleanup) {
        throw InvalidPlayError{"This turn already ended."};
    }
    m_internal.phase = TurnPhase::Cleanup;

    deck()->moveAllCards(Areas::InPlay, Areas::DiscardPile);
    deck()->moveAllCards(Areas::Hand, Areas::DiscardPile);

    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::TotalCards, deck()->totalCards());
    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::TotalMoney, deck()->totalMoney());
    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::TotalScore, deck()->countScore());
    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::TurnPeakMoney, m_internal.m_maxMoney);
    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::CardsSeen, m_internal.m_totalCardsSeen);
    Logger::instance()->addData(deck()->playerIndex(), PerTurnLogData::TurnNumber, deck()->turnCount());

    doFinalDraw();

    m_internal.phase = TurnPhase::Ended;
}

int Turn::doFinalDraw()
{
    if (currentPhase() > TurnPhase::DrawNext) {
        throw InvalidPlayError{"Attempting to draw next hand twice"};
    }
    m_internal.phase = TurnPhase::DrawNext;
    deck()->countTurn();

    return m_internal.draw(5);
}

int Turn::turnCount()
{
    return deck()->turnCount();
}

Cost Turn::cardCost(CardId id) const
{
    return m_internal.cardCost(id);
}

int TurnInternal::draw(int n)
{
    int ret = deck->drawCards(n);
    m_totalCardsSeen += ret;
    return ret;
}

void TurnInternal::trashFromHand(Card* card)
{
    deck->trash(card, Areas::Hand);
}

void TurnInternal::discardFromHand(Card* card)
{
    deck->discardFromHand(card);
}

int TurnInternal::countCardsInHand() const
{
    return static_cast<const Deck*>(deck)->hand().cards().size();
}

Cost TurnInternal::cardCost(CardId id) const
{
    auto& pile = supply->pile(id);
    if (pile.empty()) {
        return {0}; // cannot gain this anyways
    }
    auto cost = pile.topCard()->basicInfo().cost;
    auto adjCost = Cost(cost.gold() - m_discount);
    return adjCost;
}

void TurnInternal::attackEachEnemy(AttackReactOption::Factory attackFactory)
{
    // This is slightly complicated. The card itself constructs the attack parameter,
    // which is actually a description of what options the attacked player has in *reaction*
    // to what is happening. An example why this makes sense is e.g. Torturer: the Actor
    // needs to have the choice whether to discard cards or take a curse.
    // We then here wrap this attack into an event, and ask cards whether they can react to
    // an attack event. In response, they might produce a *different* kind of react option,
    // which e.g. enables the Actor to ignore the attack (Moat), or do something else before the attack.
    for (auto* enemy: deck->enemies()) {
        auto attack = attackFactory(enemy);
        auto event = AttackEvent(attack);
        enemy->attacked(event);
    }
}

