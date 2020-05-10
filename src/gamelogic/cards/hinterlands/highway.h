#pragma once

#include "turn.h"

class Highway: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(1);
        turn->addActions(1);
        turn->addDiscount(1);
    }

protected:
    friend class Supply;
    Highway() {
        m_info = {
            CardId::Highway,
            Card::Action,
            Cantrip,
            Cost{5}
        };
    }
};

