#pragma once

#include "turn.h"

template<int COST, int VALUE, CardId ID>
class PlainTreasure : public Card {
public:
    virtual BasicInfo basicInfo() const override {
        return {
            ID,
            Card::Treasure,
            NoHints,
            Cost{COST}
        };
    }

    virtual CardTraits traits() const override {
        return CardTraits{
            VALUE
        };
    };

    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        turn->money += VALUE;
    }
};

using Gold = PlainTreasure<6, 3, CardId::Gold>;
