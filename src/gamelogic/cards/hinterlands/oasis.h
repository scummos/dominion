#pragma once

#include "turn.h"

struct CardOptionOasis : public CardOption {
    std::function<Card*(Hand const&)> discard;
};

class Oasis : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionOasis*>(option_);
        if (!option || !option->discard) {
            throw InvalidCardUsage{"Oasis must discard a card."};
        }
        turn->addActions(1);
        turn->addMoney(1);
        turn->draw(1);

        auto hand = turn->turn->currentHand();
        turn->discardFromHand(option->discard(hand));
    }

protected:
    friend class Supply;
    Oasis() {
        m_info = {
            CardId::Oasis,
            Card::Action,
            VirtualMoney | Draw | Chain | Cantrip | Choice,
            Cost{3}
        };
    }
};

