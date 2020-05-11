#pragma once

#include "turn.h"

struct CardOptionUpgrade : public CardOption {
    DiscardFunc trash;
    CardTransformFunc transform;
};

class Upgrade: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->draw(1);
        turn->addActions(1);

        auto* option = static_cast<CardOptionUpgrade*>(option_);
        if (!option->trash || option->transform) {
            throw InvalidPlayError{"Invalid options for Upgrade"};
        }

        // Since we draw a card first, we need a functor to select which card to trash.
        // See what it comes up with.
        auto trash = option->trash(turn->deck->constHand().cards());
        if (trash.empty()) {
            throw InvalidPlayError{"Didn't select a card for Upgrade"};
        }
        auto gain = option->transform(trash.front()->id());

        // Check card rules.
        auto oldCost = trash.front()->cost().gold();
        auto newCost = turn->cardCost(gain).gold();
        if (newCost != oldCost + 1) {
            throw InvalidPlayError{"Upgrade requires newPrice = oldPrice + 1"};
        }

        // Do upgrade.
        turn->trashFromHand(trash.front());
        turn->deck->gainFromSupply(gain);
    }

protected:
    friend class Supply;
    Upgrade() {
        m_info = {
            CardId::Upgrade,
            Card::Action,
            Choice | Chain | Cantrip | Trasher,
            Cost{5}
        };
    }
};




