#pragma once

#include "turn.h"

class Harem : public Card {
public:

    virtual CardTraits traits() const override {
        return CardTraits{
            2
        };
    };

    void playTreasure(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addMoney(2);
    }

    int victoryPoints(Deck const*) const override {
        return 2;
    }

protected:
    friend class Supply;
    Harem() {
        m_info = {
            CardId::Harem,
            Card::Treasure | Card::Victory,
            NoHints,
            6
        };
    }
};
