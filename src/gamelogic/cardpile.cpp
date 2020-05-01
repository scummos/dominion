#include "cardpile.h"

#include <algorithm>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

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
    Cards ret;
    ret.reserve(n);
    for (int i = 0; i < n && !(empty() && reserve.empty()); i++) {
        if (!empty()) {
            ret.push_back(draw());
        }
        else {
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

int CardPile::count() const
{
    return m_cards.size();
}

int CardPile::count(CardId id) const
{
    return std::count_if(m_cards.begin(), m_cards.end(), [id](Card* c) { return c->basicInfo().id == id; });
}

Cards& CardPile::cards()
{
    return m_cards;
}

const Cards& CardPile::cards() const
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

void CardPile::moveCardTo(Card* card, CardPile& target)
{
    auto it = std::find(m_cards.begin(), m_cards.end(), card);
    if (it == m_cards.end()) {
        throw CardMoveError{"Card not in pile."};
    }

    m_cards.erase(it);
    target.put(card);
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

void CardPile::put(Cards cards)
{
    m_cards.reserve(m_cards.size() + cards.size());
    for (auto* card: cards) {
        put(card);
    }
}

void CardPile::shuffle()
{
    std::shuffle(m_cards.begin(), m_cards.end(), gen);
}
