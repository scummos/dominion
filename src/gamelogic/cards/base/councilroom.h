#pragma once

#include "turn.h"

class CouncilRoom: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(4);
        turn->addBuys(1);

        for (auto* enemy: turn->deck->enemies()) {
            enemy->drawCards(1);
        }
    }

protected:
    friend class Supply;
    CouncilRoom() {
        m_info = {
            CardId::CouncilRoom,
            Card::Action,
            TerminalDraw,
            Cost{5}
        };
    }
};

