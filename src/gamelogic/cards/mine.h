#pragma once

#include "turn.h"

struct CardOptionMine : public CardOption {
    Card* trash = nullptr;
    CardId gain = CardId::Copper;
};

class Mine: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionMine*>(option_);

        if (!option->trash) {
            // valid play: "You may trash ... "
            return;
        }

        if (!option->trash->hasType(Card::Treasure)) {
            throw InvalidCardUsage{"Mine has to be used on a treasure card."};
        }

        auto pile = turn->supply->pile(option->gain);
        if (pile.empty()) {
            // XXX missing case: what if all money piles are empty?
            throw InvalidCardUsage{"You have to select a non-empty pile to gain from."};
        }

        if (!pile.topCard()->hasType(Card::Treasure)) {
            throw InvalidCardUsage{"Mine only gains treasures."};
        }

        auto oldCost = turn->cardCost(option->trash->basicInfo().id).gold();
        auto newCost = turn->cardCost(option->gain).gold();
        if (newCost > oldCost + 3) {
            throw InvalidCardUsage{"Mine only gains Treasures costing up to 3 more."};
        }

        turn->trashFromHand(option->trash);
        turn->deck->gainFromSupply(option->gain, Areas::Hand);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Mine,
            Card::Action,
            Choice,
            Cost{5}
        };
    }
};

