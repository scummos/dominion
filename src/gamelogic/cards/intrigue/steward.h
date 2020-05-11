#pragma once

#include "turn.h"

struct CardOptionSteward : public CardOption {
    enum Choice {
        TwoCards = 1,
        TwoMoney,
        TrashTwo
    };
    Choice choice = TwoCards;
    DiscardFunc trash;
};

class Steward: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionSteward*>(option_);
        switch (option->choice) {
            case CardOptionSteward::TwoCards:
                turn->draw(2);
                break;

            case CardOptionSteward::TwoMoney:
                turn->addMoney(2);
                break;

            case CardOptionSteward::TrashTwo: {
                if (!option->trash) {
                    throw InvalidPlayError{"Choosing Trash with Steward requires specifying what cards to trash"};
                }
                auto trash = option->trash(turn->turn->currentHandCards());
                if (trash.size() < 2 && trash.size() != turn->turn->currentHandCards().size()) {
                    throw InvalidPlayError{"Choosing Trash with Steward requires trashing exactly 2 cards"};
                }
                turn->trashFromHand(trash.at(0));
                turn->trashFromHand(trash.at(1));
                break;
            }

            default:
                throw InvalidPlayError{"Invalid choice for Steward"};
        }
    }

protected:
    friend class Supply;
    Steward() {
        m_info = {
            CardId::Steward,
            Card::Action,
            Choice,
            Cost{3}
        };
    }
};






