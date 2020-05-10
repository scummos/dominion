#pragma once

#include "card.h"

class Curse : public Card {
public:
    int victoryPoints() const override {
        return -1;
    }

protected:
    friend class Supply;
    Curse() {
        m_info = {
            CardId::Curse,
            Card::NoType,
            NoHints,
            Cost{0}
        };
    }
};
