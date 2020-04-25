#pragma once

#include "turn.h"
#include "reaction.h"

class Witch: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(2);

        auto opt = std::make_shared<GainAttackReactOption>(CardId::Curse, 1);
        turn->attackEachEnemy(opt);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Witch,
            Card::Action,
            TerminalDraw | Attack,
            Cost{5}
        };
    }
};
