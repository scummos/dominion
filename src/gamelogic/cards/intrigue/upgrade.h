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
        if (!option->trash || !option->transform) {
            throw InvalidPlayError{"Invalid options for Upgrade"};
        }

        // Since we draw a card first, we need a functor to select which card to trash.
        // See what it comes up with.
        auto trash = option->trash(turn->deck->constHand().cards());
        if (trash.empty()) {
            throw InvalidPlayError{"Didn't select a card for Upgrade"};
        }
        auto* trashCard = trash.front();
        auto gain = option->transform(trashCard->id());

        // Check card rules.
        auto oldCost = trashCard->cost();
        auto requiredCost = oldCost;
        requiredCost.m_gold += 1;
        if (turn->deck->supply()->getAnyWithCost(requiredCost) != CardId::Invalid) {
            if (gain == CardId::NoCard) {
                throw InvalidPlayError{"Cannot trash card with Upgrade when there is a card that could be gained"};
            }
            auto newCost = turn->cardCost(gain);
            if (newCost != requiredCost) {
                throw InvalidPlayError{"Upgrade requires newGoldPrice = oldGoldPrice + 1"};
            }
        }

        // Do upgrade.
        turn->trashFromHand(trashCard);
        if (gain != CardId::NoCard) {
            turn->deck->gainFromSupply(gain);
        }
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




