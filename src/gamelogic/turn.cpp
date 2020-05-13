#include "turn.h"

#include "error.h"
#include "logger.h"

#include <algorithm>
#include <utility>

using std::operator""s;

#define do_log(x) if(loggingEnabled()) log(x);
#define do_log_internal(x) if(turn->loggingEnabled()) turn->log(x);

Turn::Turn(Supply* supply, Deck* deck, Logger::PlayerData& logData, Game::LogFunction func)
    : m_logData(logData)
    , m_logFunc(func)
{
    m_internal.turn = this;
    m_internal.supply = supply;
    m_internal.deck = deck;
    m_internal.m_totalCardsSeen = deck->constHand().cards().size();
}

ActiveCards Hand::treasureCards() const
{
    ActiveCards ret;
    for (auto const& c: cards) {
        if (c != ignored && c->hasType(Card::Treasure)) {
            ret.emplace_back(ActiveCard{turn, c});
        }
    }
    return ret;
}

void Hand::ignore(Card* card)
{
    ignored = card;
}

ActiveCards Hand::findCards(CardId id) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card != ignored && card->basicInfo().id == id) {
            ret.emplace_back(ActiveCard{turn, card});
        }
    }
    return ret;
}

ActiveCards Hand::findCards(Card::Type type) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card != ignored && card->hasType(type)) {
            ret.emplace_back(ActiveCard{turn, card});
        }
    }
    return ret;
}

ActiveCards Hand::findCards(Card::Hints hints) const
{
    ActiveCards ret;
    for (auto const& card: cards) {
        if (card != ignored && card->hints() & hints) {
            ret.emplace_back(ActiveCard{turn, card});
        }
    }
    return ret;
}

bool Hand::hasCard(CardId id) const
{
    return std::any_of(cards.begin(), cards.end(), [id, this](Card* c) {
        return c != ignored && c->basicInfo().id == id;
    });
}

bool Hand::hasCard(Card::Type type) const
{
    return std::any_of(cards.begin(), cards.end(), [type, this](Card* c) {
        return c!= ignored && c->types() & type;
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

    do_log("Buy card: "s + cardName(id) + " from " + std::to_string(m_internal.money()));

    m_internal.addBuys(-1);
    m_internal.addMoney(-cost.gold());

    deck()->gainFromSupply(id, Areas::DiscardPile, GainReason::Buy);
}

Hand::Hand(const Cards& cards, Turn* turn)
    : cards(cards)
    , turn(turn)
{
}

ActiveCards Hand::activeCards() const
{
    ActiveCards ret;
    std::transform(cards.begin(), cards.end(), std::back_inserter(ret), [this](Card* c) { return ActiveCard{turn, c}; });
    return ret;
}

ActiveCard Hand::activeCard(Card* card) const
{
    return ActiveCard{turn, card};
}

Hand Turn::currentHand()
{
    return Hand(
        m_internal.deck->constHand().cards(),
        this
    );
}

Cards Turn::currentHandCards()
{
    return m_internal.deck->constHand().cards();
}

int Turn::currentHandSize() const {
    return m_internal.deck->constHand().count();
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

int Turn::emptySupplyPiles() const
{
    return m_internal.supply->countEmptyPiles();
}

int Turn::currentActions() const
{
    return m_internal.actions();
}

int Turn::currentBuys() const
{
    return m_internal.buys();
}

void Turn::log(std::string message)
{
    if (!loggingEnabled()) {
        return;
    }
    m_logFunc(m_internal.deck->playerIndex(), message);
}

bool Turn::loggingEnabled() const
{
    return m_logFunc != nullptr;
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
    m_internal.m_numPlayed[card->id()]++;

    do_log("Play action: "s + card->name());

    card->playAction(&m_internal, option);
}

void Turn::playTreasure(Card* card, CardOption* option)
{
    if (currentPhase() > TurnPhase::PlayTreasures) {
        throw InvalidPlayError{"You already finished playing treasures and cannot play any more."};
    }
    m_internal.phase = TurnPhase::PlayTreasures;

    deck()->moveCard(card, Areas::Hand, Areas::InPlay);
    m_internal.m_numPlayed[card->id()]++;

    card->playTreasure(&m_internal, option);
}

void Turn::endTurn()
{
    if (currentPhase() > TurnPhase::Cleanup) {
        throw InvalidPlayError{"This turn already ended."};
    }
    m_internal.phase = TurnPhase::Cleanup;

    m_logData.addData(PerTurnLogData::TotalCards, deck()->totalCards());
    m_logData.addData(PerTurnLogData::TotalMoney, deck()->totalMoney());
    m_logData.addData(PerTurnLogData::TotalScore, deck()->countScore());
    m_logData.addData(PerTurnLogData::TurnPeakMoney, m_internal.m_maxMoney);
    m_logData.addData(PerTurnLogData::CardsSeen, m_internal.m_totalCardsSeen);
    m_logData.addData(PerTurnLogData::TurnNumber, deck()->turnCount());
    m_logData.addData(PerTurnLogData::Curses, deck()->totalCards(CardId::Curse));
    m_logData.addData(PerTurnLogData::UnplayedActions, m_internal.countCardsInHand(Card::Action));
    m_logData.addData(PerTurnLogData::UnusedBuys, m_internal.buys());
    m_logData.addData(PerTurnLogData::UnusedMoney, m_internal.money());
    m_logData.addData(PerTurnLogData::UnusedActions, m_internal.actions());

    deck()->moveAllCards(Areas::InPlay, Areas::DiscardPile);
    deck()->moveAllCards(Areas::Hand, Areas::DiscardPile);

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

int TurnInternal::numPlayed(CardId card) const
{
    return m_numPlayed.find(card)->second;
}

int TurnInternal::numPlayed(Card::Type type) const
{
    int ret = 0;
    for (auto const& entry: m_numPlayed) {
        auto const info = supply->pileInfo(entry.first);
        if (info.types & type) {
            ret += entry.second;
        }
    }
    return ret;
}

int TurnInternal::draw(int n)
{
    int ret = deck->drawCards(n);
    m_totalCardsSeen += ret;
    if (turn->currentPhase() != TurnPhase::DrawNext) {
        do_log_internal("Draw cards: "s + std::to_string(n) + ", new hand: " + formatCardList(deck->constHand().cards()));
    }
    return ret;
}

void TurnInternal::trashFromHand(Card* card)
{
    deck->trash(card, Areas::Hand);
    do_log_internal("Trash card: "s + card->name());
}

void TurnInternal::discardFromHand(Card* card)
{
    deck->discardFromHand(card);
    do_log_internal("Discard: "s + card->name());
}

int TurnInternal::countCardsInHand() const
{
    return deck->constHand().cards().size();
}

int TurnInternal::countCardsInHand(Card::Type type) const
{
    return deck->constHand().count(type);
}

Cost TurnInternal::cardCost(CardId id) const
{
    auto cost = supply->pileInfo(id).cost;
    auto adjCost = Cost(std::max(0, cost.gold() - m_discount));
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

