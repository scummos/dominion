#pragma once

#include "turn.h"

class Crossroads: public Card {
public:
    void playAction(TurnInternal* turn, CardOption*) override {
        auto draw = turn->turn->currentHand().findCards(Card::Victory).size();
        turn->draw(draw);
        if (turn->numPlayed(CardId::Crossroads) == 1) {
            turn->addActions(3);
        }
    }

protected:
    friend class Supply;
    Crossroads() {
        m_info = {
            CardId::Crossroads,
            Card::Action,
            Chain | Draw,
            Cost{2}
        };
    }
};
