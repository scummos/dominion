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

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Moneylender,
            Card::Action,
            VirtualMoney,
            Cost{4}
        };
    }
};



