#pragma once

#include "card.h"

template<int COST, int VP, CardId ID>
class PlainVictory : public Card {
public:
    int victoryPoints() const override {
        return VP;
    }

protected:
    friend class Supply;
    PlainVictory() {
        m_info = {
            ID,
            Card::Victory,
            NoHints,
            Cost{COST}
        };
    }
};

using Province = PlainVictory<8, 6, CardId::Province>;
