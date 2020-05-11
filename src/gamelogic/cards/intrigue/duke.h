#pragma once

#include "card.h"
#include "deck.h"

class Duke : public Card {
public:
    int victoryPoints(Deck const* deck) const override {
        return deck->totalCards(CardId::Duchy);
    }

protected:
    friend class Supply;
    Duke() {
        m_info = {
            CardId::Duke,
            Card::Victory,
            NoHints,
            Cost{5}
        };
    }
};
