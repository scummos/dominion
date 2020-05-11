#pragma once

#include "turn.h"

struct CardOptionTradingPost : public CardOption {
    std::vector<Card*> trash;
};

class TradingPost: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionTradingPost*>(option_);
        if (option->trash.size() != 2) {
            throw InvalidPlayError{"Trading post trashes exactly 2 cards"};
        }

        for (auto* card: option->trash) {
            turn->trashFromHand(card);
        }

        turn->deck->gainFromSupply(CardId::Silver, Areas::Hand);
    }

protected:
    friend class Supply;
    TradingPost() {
        m_info = {
            CardId::TradingPost,
            Card::Action,
            Choice | Trasher,
            Cost{5}
        };
    }
};


