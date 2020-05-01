#pragma once

#include "turn.h"

class FoolsGold : public Card {
public:
    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        // TODO not quite correct: this shoult not count cards in play, but how often you
        // played a Fool's Gold this turn
        // We have to check > 1, because this card already is in play when this function
        // is called.
        if (static_cast<const Deck*>(turn->deck)->inPlay().count(CardId::FoolsGold) > 1) {
            turn->addMoney(4);
        }
        else {
            turn->addMoney(1);
        }
    }

protected:
    friend class Supply;
    FoolsGold() {
        m_info = {
            CardId::FoolsGold,
            Card::Treasure,
            NoHints,
            Cost{2}
        };
    }
};

