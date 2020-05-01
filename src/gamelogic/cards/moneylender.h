#pragma once

#include "turn.h"

class Moneylender : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option_*/) override {
        auto coppers = turn->turn->currentHand().findCards(CardId::Copper);
        if (coppers.empty()) {
            return;
        }
        turn->trashFromHand(coppers.front().card);
        turn->addMoney(3);
    }

protected:
    friend class Supply;
    Moneylender() {
        m_info = {
            CardId::Moneylender,
            Card::Action,
            VirtualMoney,
            Cost{4}
        };
    }
};



