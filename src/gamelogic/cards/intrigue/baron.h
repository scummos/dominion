#pragma once

#include "turn.h"

struct CardOptionBaron : public CardOption {
    bool doDiscard = false;
};

class Baron: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionBaron*>(option_);
        if (option->doDiscard) {
            auto estates = turn->turn->currentHand().findCards(CardId::Estate);
            if (!estates.empty()) {
                turn->discardFromHand(estates.front().card);
                turn->addMoney(4);
            }
        }
        else {
            turn->deck->gainFromSupply(CardId::Estate);
        }
    }

protected:
    friend class Supply;
    Baron() {
        m_info = {
            CardId::Baron,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};





