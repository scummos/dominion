#pragma once

#include "turn.h"

class Market : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addActions(1);
        turn->addMoney(1);
        turn->addBuys(1);
        turn->draw(1);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Market,
            Card::Action,
            VirtualMoney | Draw | Chain | Cantrip | PlusBuy,
            Cost{5}
        };
    }
};


