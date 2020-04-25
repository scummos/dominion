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

    void std::shared_ptr<EventReactOption> reactToEvent(Event& event) const override {
        if (!event.kind & EventKind::Attack) {
            return {};
        }
        return std::make_shared<IgnoreAttackReactOption>(event);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Moat,
            Card::Action,
            TerminalDraw,
            Cost{2}
        };
    }
};

