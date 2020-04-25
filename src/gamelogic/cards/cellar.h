#pragma once

#include "turn.h"

struct CardOptionCellar : public CardOption {
    std::vector<Card*> discard;
};

class Cellar: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionCellar*>(option_);

        for (auto* card: option->discard) {
            turn->discardFromHand(card);
        }

        turn->draw(option->discard.size());
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Cellar,
            Card::Action,
            Choice | Chain | Draw,
            Cost{2}
        };
    }
};


