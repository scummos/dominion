#pragma once

#include "turn.h"

struct CardOptionWorkshop : public CardOption {
    CardId gain = CardId::Copper;
};

class Workshop : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionWorkshop*>(option_);
        auto cost = turn->cardCost(option->gain);
        if (cost.hasAdvancedCost() || cost.gold() > 4) {
            throw InvalidPlayError{"Workshop only gains cards costing up to 4."};
        }
        turn->deck->gainFromSupply(turn->supply, option->gain);
    }

protected:
    friend class Supply;
    Workshop() {
        m_info = {
            CardId::Workshop,
            Card::Action,
            Choice,
            Cost{3}
        };
    }
};


