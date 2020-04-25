#pragma once

#include "card.h"

class Curse : public Card {
public:
    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Curse,
            Card::NoType,
            NoHints,
            Cost{0}
        };
    }

    int victoryPoints() const override {
        return -1;
    }
};
