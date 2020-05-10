#pragma once

#include "turn.h"

class Laboratory : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addActions(1);
        turn->draw(2);
    }

protected:
    friend class Supply;
    Laboratory() {
        m_info = {
            CardId::Laboratory,
            Card::Action,
            Draw | Chain,
            Cost{5}
        };
    }
};



