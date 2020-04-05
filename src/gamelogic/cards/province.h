#pragma once

#include "card.h"

template<int COST, int VP, CardId ID>
class PlainVictory : public Card {
public:
    virtual BasicInfo basicInfo() const override {
        return {
            ID,
            Card::Victory,
            NoHints,
            Cost{COST}
        };
    }

    int victoryPoints() const override {
        return VP;
    }
};

using Province = PlainVictory<8, 6, CardId::Province>;
