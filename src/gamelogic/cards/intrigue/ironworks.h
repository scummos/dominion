#pragma once

#include "turn.h"

struct CardOptionIronworks : public CardOption {
    CardId gain = CardId::Silver;
};

class Ironworks: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionIronworks*>(option_);
        auto cost = turn->cardCost(option->gain);
        if (cost.hasAdvancedCost() || cost.gold() > 4) {
            throw InvalidPlayError{"Ironworks gains cards costing up to 4 only"};
        }
        turn->deck->gainFromSupply(option->gain);

        auto info = turn->supply->pileInfo(option->gain);
        if (info.types & Card::Action)
            turn->addActions(1);
        if (info.types & Card::Treasure)
            turn->addMoney(1);
        if (info.types & Card::Victory)
            turn->draw(1);
    }

protected:
    friend class Supply;
    Ironworks() {
        m_info = {
            CardId::Ironworks,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};


