#pragma once

#include "turn.h"

class FoolsGold : public Card {
public:
    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        if (turn->numPlayed(CardId::FoolsGold) > 1) {
            turn->addMoney(4);
        }
        else {
            turn->addMoney(1);
        }
    }

protected:
    friend class Supply;
    FoolsGold() {
        m_info = {
            CardId::FoolsGold,
            Card::Treasure,
            NoHints,
            Cost{2}
        };
    }
};

