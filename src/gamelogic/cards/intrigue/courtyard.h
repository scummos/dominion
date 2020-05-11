#pragma once

#include "turn.h"
#include "event_helpers.h"

struct CardOptionCourtyard : public CardOption {
    DiscardFunc putBack;
};

class Courtyard: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->draw(3);

        auto* option = static_cast<CardOptionCourtyard*>(option_);
        auto hand = turn->turn->currentHandCards();
        auto discard = option->putBack(hand);
        if (discard.size() == 0) {
            throw InvalidPlayError{"Courtyard needs exactly 1 card to put back"};
        }
        turn->deck->moveCard(discard.front(), Areas::Hand, Areas::DrawPile);
    }

protected:
    friend class Supply;
    Courtyard() {
        m_info = {
            CardId::Courtyard,
            Card::Action,
            Draw | Choice,
            Cost{2}
        };
    }
};




