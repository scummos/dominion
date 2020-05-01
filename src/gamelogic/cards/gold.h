#pragma once

#include "turn.h"

template<int COST, int VALUE, CardId ID>
class PlainTreasure : public Card {
public:

    virtual CardTraits traits() const override {
        return CardTraits{
            VALUE
        };
    };

    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addMoney(VALUE);
    }

protected:
    friend class Supply;
    PlainTreasure() {
        m_info = {
            ID,
            Card::Treasure,
            NoHints,
            Cost{COST}
        };
    }
};

using Gold = PlainTreasure<6, 3, CardId::Gold>;
