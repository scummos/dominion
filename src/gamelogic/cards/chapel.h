#pragma once

#include "turn.h"

struct CardOptionChapel : public CardOption {
    std::vector<Card*> trash;
};

class Chapel: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionChapel*>(option_);
        if (option->trash.size() > 4) {
            throw InvalidPlayError{"Chapel trashes at most 4 cards"};
        }

        for (auto* card: option->trash) {
            turn->trashFromHand(card);
        }
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Chapel,
            Card::Action,
            Choice | Trasher,
            Cost{2}
        };
    }
};

