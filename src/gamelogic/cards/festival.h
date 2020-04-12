#pragma once

#include "turn.h"

class Festival: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addActions(2);
        turn->addMoney(2);
        turn->addBuys(1);
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

