#pragma once

#include "cardpile.h"

struct NoSuchPileError {
    std::string error;
};

class Supply {
public:
    Supply();
    ~Supply();

    template<typename T>
    CardPile* pile();

    int countEmptyPiles() const;

protected:
    template<typename T>
    void createPile(int count);

private:
    std::vector<CardPile> m_piles;
};

// template implementations
template<typename T> void Supply::createPile(int count)
{
    static_assert(sizeof(T) == sizeof(Card), "invalid Card type");

    CardPile pile;
    for (int i = 0; i < count; i++) {
        pile.put(new T);
    }
    m_piles.emplace_back(std::move(pile));
}

template<typename T> CardPile* Supply::pile()
{
    for (auto& pile: m_piles) {
        if (!pile.empty() && dynamic_cast<T*>(pile.topCard())) {
            return &pile;
        }
    }
    throw NoSuchPileError{"The requested pile is empty or does not exist."};
}
