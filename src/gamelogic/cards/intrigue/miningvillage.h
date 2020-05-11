#pragma once

#include "turn.h"

struct CardOptionMiningVillage : public CardOption {
    bool doTrash = false;
};

class MiningVillage: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->addActions(2);
        turn->draw(1);

        auto* option = static_cast<CardOptionMiningVillage*>(option_);
        if (option->doTrash) {
            turn->deck->trash(this, Areas::InPlay);
            turn->addMoney(2);
        }
    }

protected:
    friend class Supply;
    MiningVillage() {
        m_info = {
            CardId::MiningVillage,
            Card::Action,
            Choice | Chain | Draw | Village,
            Cost{4}
        };
    }
};
