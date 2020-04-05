#pragma once

#include "turn.h"

class Smithy: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(3);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Smithy,
            Card::Action,
            TerminalDraw,
            Cost{4}
        };
    }
};
