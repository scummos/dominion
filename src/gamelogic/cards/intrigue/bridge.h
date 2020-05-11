#pragma once

#include "turn.h"

class Bridge: public Card {
public:
    void playAction(TurnInternal* turn, CardOption*) override {
        turn->addBuys(1);
        turn->addMoney(1);
        turn->addDiscount(1);
    }

protected:
    friend class Supply;
    Bridge() {
        m_info = {
            CardId::Bridge,
            Card::Action,
            VirtualMoney | PlusBuy | Discount,
            Cost{4}
        };
    }
};
