#pragma once

#include "turn.h"
#include <algorithm>

struct CardOptionShepherd : public CardOption {
    std::vector<Card*> discard;
};

class Shepherd: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionShepherd*>(option_);
        auto isNonVictory = [] (Card* card) {
            return !card->hasType(Card::Victory);
        };
        if (std::any_of(option->discard.begin(), option->discard.end(), isNonVictory)) {
            throw InvalidPlayError{"Shepherd discards only Victory cards"};
        }

        for (auto* card: option->discard) {
            turn->discardFromHand(card);
        }
        turn->draw(2 * option->discard.size());
        turn->addActions(1);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Shepherd,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};

