#pragma once

#include "turn.h"

class Village: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addActions(2);
        turn->draw(1);
    }

protected:
    friend class Supply;
    Village() {
        m_info = {
            CardId::Village,
            Card::Action,
            Chain | Draw,
            Cost{3}
        };
    }
};


