#pragma once

#include "turn.h"

class ShantyTown: public Card {
public:
    void playAction(TurnInternal* turn, CardOption*) override {
        turn->addActions(2);

        if (!turn->turn->currentHand().hasCard(Card::Action)) {
            turn->draw(2);
        }
    }

protected:
    friend class Supply;
    ShantyTown() {
        m_info = {
            CardId::ShantyTown,
            Card::Action,
            Chain,
            Cost{3}
        };
    }
};
