#pragma once

#include "turn.h"
#include "reaction.h"
#include "event.h"

#include <memory>

class Moat: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(2);
    }

    EventReactOption::Ptr reactToEvent(Event& event, Deck*, Areas area) const override {
        if (area != Areas::Hand | event.kind != EventKind::Attack) {
            return {};
        }
        return std::make_shared<IgnoreAttackReactOption>(event);
    }

protected:
    friend class Supply;
    Moat() {
        m_info = {
            CardId::Moat,
            Card::Action,
            TerminalDraw,
            Cost{2}
        };
    }
};

