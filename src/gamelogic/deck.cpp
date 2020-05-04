#include "deck.h"

#include "error.h"

#include <numeric>
#include <algorithm>
#include <iostream>

CardPile const& Deck::area(Areas area) const
{
    return m_areas.at(static_cast<int>(area));
}

CardPile& Deck::area(Areas area)
{
    return m_areas.at(static_cast<int>(area));
}

Deck::Deck(std::vector<Card*> startingCards, Supply* supply, int playerIndex)
    : m_supply(supply)
    , m_playerIndex(playerIndex)
{
    m_areas.resize(static_cast<int>(Areas::NumAreas));

    for (auto* card: startingCards) {
        drawPile().put(card);
    }
    drawPile().shuffle();
}

Deck::Deck(Deck && other)
{
    m_areas = other.m_areas;
    other.m_areas.clear();
    m_supply = other.m_supply;
    m_enemies = other.m_enemies;
    m_turnCount = other.m_turnCount;
    m_playerIndex = other.m_playerIndex;
    m_react = other.m_react;
}

Deck::~Deck()
{
    for (auto& pile: m_areas) {
        std::for_each(pile.cards().begin(), pile.cards().end(), [](Card* c) { delete c; });
    }
}

int Deck::drawCards(int n)
{
    auto cards = drawPile().drawWithShuffle(n, discardPile());
    hand().put(cards);
    return cards.size();
}

int Deck::uncoverCards(int n)
{
    auto cards = drawPile().drawWithShuffle(n, discardPile());
    uncoveredDrawPile().put(cards);
    return cards.size();
}

void Deck::putUncoveredBack()
{
    uncoveredDrawPile().moveAllTo(drawPile());
}

void Deck::trash(Card* card, Areas sourceArea)
{
    area(sourceArea).moveCardTo(card, m_supply->trashPile());
}

void Deck::moveCard(int index, Areas from, Areas to)
{
    area(from).moveCardTo(index, area(to));
}

void Deck::moveCard(Card* card, Areas from, Areas to)
{
    area(from).moveCardTo(card, area(to));
}

void Deck::moveAllCards(Areas from, Areas to)
{
    area(from).moveAllTo(area(to));
}

void Deck::discardFromHand(Card* card)
{
    auto event = CardDiscardedFromHandEvent(card);
    auto opts = queryCardsForReactions(event);
    if (m_react && !opts.empty()) {
        m_react(opts);
    }

    if (!event.ignored) {
        moveCard(card, Areas::Hand, Areas::DiscardPile);
    }
}

bool Deck::gainFromSupply(const CardId id, Areas targetArea)
{
    auto& pile = m_supply->pile(id);
    if (pile.empty()) {
        return false;
    }

    auto event = YouGainACardEvent(pile.topCard());
    auto opts = queryCardsForReactions(event);

    // the gained card can also react
    auto gainedReact = event.card->reactToEvent(event, this);
    if (gainedReact) {
        opts.push_back(gainedReact);
    }
    if (m_react && !opts.empty()) {
        m_react(opts);
    }

    for (auto& opt: opts) {
        opt->defaultAction();
    }

    if (!event.ignored) {
        pile.moveCardTo(0, area(targetArea));
    }

    EnemyGainsACardEvent notify(event.card);
    for (auto* other: enemies()) {
        other->eventOccured(notify);
    }

    return true;
}

int Deck::totalCards() const
{
    return std::accumulate(m_areas.begin(), m_areas.end(), 0, [](int k, auto const& v) { return k + v.count(); });
}

void Deck::forEachCard(std::function<void(Card const*)> func) const
{
    std::for_each(m_areas.begin(), m_areas.end(), [&func](auto const& area) {
        std::for_each(area.cards().begin(), area.cards().end(), func);
    });
}

int Deck::totalCards(CardId id) const
{
    int count = 0;
    forEachCard([&count, id](Card const* c) {
        if (c->basicInfo().id == id) {
            count++;
        }
    });
    return count;
}

int Deck::countScore() const
{
    int score = 0;
    forEachCard([&score](Card const* card) {
        score += card->victoryPoints();
    });
    return score;
}

int Deck::totalMoney() const
{
    int money = 0;
    forEachCard([&money](Card const* card) {
        money += card->traits().treasureValue;
    });
    return money;
}

void Deck::countTurn()
{
    m_turnCount++;
}

int Deck::turnCount() const
{
    return m_turnCount;
}

int Deck::playerIndex() const
{
    return m_playerIndex;
}

std::vector<Deck*> Deck::enemies() const
{
    return m_enemies;
}

void Deck::setEnemies(std::vector<Deck *> enemies)
{
    m_enemies = enemies;
}

void Deck::setReactCallback(Deck::ReactCallback cb)
{
    m_react = cb;
}

EventReactOptions Deck::queryCardsForReactions(Event& event)
{
    EventReactOptions opts;

    for (auto* card: hand().cards()) {
        auto opt = card->reactToEvent(event, this);
        if (!opt) {
            // Card doesn't react to (this type of?) event, or at least doesn't offer options
            continue;
        }
        opts.push_back(opt);
    }

    return opts;
}

void Deck::eventOccured(Event& event)
{
    // Look what cards we have to react to this event.
    auto opts = queryCardsForReactions(event);

    // Give the actor the chance to parametrize (accept, ignore, choose things) these cards.
    if (m_react && !opts.empty()) {
        m_react(opts);
    }
}

void Deck::attacked(AttackEvent& event)
{
    // First, use generic event handler
    eventOccured(event);

    // Maybe we averted the attack?
    if (event.ignored) {
        return;
    }

    // Unlucky. Give the actor the chance to parametrize the effects of the actual attack.
    if (m_react) {
        auto opts = EventReactOptions({event.attack});
        m_react(opts);
    }

    // In any case, run the default handler of the action. This must check internally if the Actor
    // has already accepted part of the effect (if he was given that opportunity by the option's API).
    event.attack->defaultAction();
}

bool Deck::hasCardInHand(Card const* card) const
{
    auto const& h = hand().cards();
    return std::find(h.begin(), h.end(), card) != h.end();
}
