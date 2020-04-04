#pragma once

#include "turn.h"

template<int COST, int VALUE>
class PlainTreasure : public Card {
public:
    Cost cost() const override {
        return COST;
    }

    Card::Type types() const override {
        return Card::Treasure;
    }

    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        turn->money += VALUE;
    }
};

using Gold = PlainTreasure<6, 3>;
