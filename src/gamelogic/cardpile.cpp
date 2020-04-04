#include "cardpile.h"

#include <algorithm>
#include <random>

CardPile::CardPile(Cards cards)
    : m_cards(cards)
{
}

Card* CardPile::draw()
{
    if (m_cards.empty())
        throw CardMoveError{"Pile is empty."};

    auto ret = topCard();
    m_cards.pop_back();
    return ret;
}

Cards CardPile::drawWithShuffle(int n, CardPile& reserve)
{
    std::vector<Card*> ret;
    for (int i = 0; i < n && !(empty() && reserve.empty()); i++) {
        try {
            ret.push_back(draw());
        }
        catch (CardMoveError) {
            reserve.moveAllTo(*this);
            shuffle();
            i--;
        }
    }
    return ret;
}

Card* CardPile::topCard()
{
    return m_cards.back();
}

Cards& CardPile::cards()
{
    return m_cards;
}

const Cards & CardPile::cards() const
{
    return m_cards;
}

bool CardPile::empty() const
{
    return m_cards.empty();
}

void CardPile::moveCardTo(int index, CardPile& target)
{
    if (m_cards.size() <= index)
        throw CardMoveError{"No such card in this pile."};

    target.put(m_cards.at(index));
    m_cards.erase(m_cards.begin() + index);
}

void CardPile::put(Card* card)
{
    m_cards.push_back(card);
}

void CardPile::moveAllTo(CardPile& target)
{
    for (auto* card: m_cards) {
        target.put(card);
    }
    m_cards.clear();
}

void CardPile::put(std::vector<Card*> cards)
{
    for (auto* card: cards) {
        put(card);
    }
}

void CardPile::shuffle()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(m_cards.begin(), m_cards.end(), gen);
}
