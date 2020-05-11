#pragma once

#include "turn.h"

struct CardOptionPawn : public CardOption {
    enum Choice {
        OneCard = 1,
        OneAction,
        OneBuy,
        OneMoney
    };
    Choice choice1 = OneCard;
    Choice choice2 = OneAction;
};

class Pawn: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionPawn*>(option_);
        auto doOption = [turn](CardOptionPawn::Choice const choice) {
            switch (choice) {
                case CardOptionPawn::OneCard:   turn->draw(1); break;
                case CardOptionPawn::OneAction: turn->addActions(1); break;
                case CardOptionPawn::OneBuy:    turn->addBuys(1); break;
                case CardOptionPawn::OneMoney:  turn->addMoney(1); break;
                default: throw InvalidPlayError{"Invalid choice for Pawn"};
            }
        };
        doOption(option->choice1);
        doOption(option->choice2);
    }

protected:
    friend class Supply;
    Pawn() {
        m_info = {
            CardId::Pawn,
            Card::Action,
            Choice,
            Cost{2}
        };
    }
};





