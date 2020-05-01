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

protected:
    friend class Supply;
    Library() {
        m_info = {
            CardId::Library,
            Card::Action,
            TerminalDraw,
            Cost{5}
        };
    }
};
