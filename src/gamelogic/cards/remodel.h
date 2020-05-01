#pragma once

#include "turn.h"

struct CardOptionRemodel : public CardOption {
    Card* trash = nullptr;
    CardId gain = CardId::Copper;
};

class Remodel: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionRemodel*>(option_);

        auto oldCost = turn->cardCost(option->trash->basicInfo().id).gold();
        auto newCost = turn->cardCost(option->gain).gold();
        if (newCost > oldCost + 2) {
            throw InvalidPlayError{"Remodel requires newPrice <= oldPrice + 2"};
        }

        turn->trashFromHand(option->trash);
        turn->deck->gainFromSupply(turn->supply, option->gain);
    }

protected:
    friend class Supply;
    Remodel() {
        m_info = {
            CardId::Remodel,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};



