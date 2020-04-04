#pragma once

#include "card.h"

template<int COST, int VP>
class PlainVictory : public Card {
public:
    Cost cost() const override {
        return COST;
    }

    Card::Type types() const override {
        return Card::Victory;
    }

    int victoryPoints() const override {
        return VP;
    }
};

using Province = PlainVictory<8, 6>;
