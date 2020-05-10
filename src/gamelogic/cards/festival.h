#pragma once

#include "turn.h"

class Festival: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addActions(2);
        turn->addMoney(2);
        turn->addBuys(1);
    }

protected:
    friend class Supply;
    Festival() {
        m_info = {
            CardId::Festival,
            Card::Action,
            Chain | VirtualMoney | PlusBuy,
            Cost{5}
        };
    }
};

