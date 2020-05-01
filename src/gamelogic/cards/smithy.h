#pragma once

#include "turn.h"

class Smithy: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(3);
    }

protected:
    friend class Supply;
    Smithy() {
        m_info = {
            CardId::Smithy,
            Card::Action,
            TerminalDraw,
            Cost{4}
        };
    }
};
