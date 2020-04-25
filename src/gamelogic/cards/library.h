#pragma once

#include "turn.h"

class Library: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        auto const mayDraw = 7 - turn->countCardsInHand();
        if (mayDraw <= 0) {
            return;
        }
        turn->draw(mayDraw);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Library,
            Card::Action,
            TerminalDraw,
            Cost{5}
        };
    }
};
