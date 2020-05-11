#pragma once

#include "turn.h"

class Conspirator: public Card {
public:
    void playAction(TurnInternal* turn, CardOption*) override {
        turn->addMoney(2);
        if (turn->numPlayed(Card::Action) >= 3) {
            turn->addActions(1);
            turn->draw(1);
        }
    }

protected:
    friend class Supply;
    Conspirator() {
        m_info = {
            CardId::Conspirator,
            Card::Action,
            VirtualMoney,
            Cost{4}
        };
    }
};

