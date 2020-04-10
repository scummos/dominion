#include "deck.h"

#include "error.h"

#include <numeric>
#include <algorithm>

CardPile const& Deck::area(Areas area) const
{
    return m_areas.at(static_cast<int>(area));
}

CardPile& Deck::area(Areas area)
{
    return m_areas.at(static_cast<int>(area));
}

Deck::Deck(std::vector<Card*> startingCards)
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

void Deck::trash(Supply* supply, Card* card, Areas sourceArea)
{
    area(sourceArea).moveCardTo(card, supply->trashPile());
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

void Deck::gainFromSupply(Supply* supply, const CardId id, Areas targetArea)
{
    auto& pile = supply->pile(id);
    if (pile.empty()) {
        throw InvalidPlayError{"This supply pile is empty."};
    }

    pile.moveCardTo(0, area(targetArea));
}

int Deck::totalCards() const
{
    return std::accumulate(m_areas.begin(), m_areas.end(), 0, [](int k, auto const& v) { return k + v.count(); });
}

int Deck::countScore() const
{
    return std::accumulate(m_areas.begin(), m_areas.end(), 0, [](int k, auto const& v) {
        return k + std::accumulate(v.cards().begin(), v.cards().end(), 0, [](int r, auto const* w) {
            return r + w->victoryPoints();
        });
    });
}
