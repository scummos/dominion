#pragma once

#include "turn.h"

class Festival: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->actions += 2;
        turn->money += 2;
        turn->buys += 1;
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Festival,
            Card::Action,
            VirtualMoney,
            Cost{5}
        };
    }
};

