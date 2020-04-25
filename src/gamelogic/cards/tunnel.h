#pragma once

#include "turn.h"
#include "event.h"

struct TunnelReactOption : public EventReactOption {
    TunnelReactOption(Deck* deck) : EventReactOption(ReactKind::Other, deck) {}

    void takeGold() {
        if (m_done) {
            return;
        }
        deck->gainFromSupply(CardId::Gold, Areas::DiscardPile);
        m_done = true;
    }

    void ignore() {
        m_done = true;
    }

    void defaultAction() override {
        // Most players probably want to do this, so do it by default unless
        // ignore() is called.
        takeGold();
    }

protected:
    bool m_done = false;
};

class Tunnel: public Card {
public:
    std::shared_ptr<EventReactOption> reactToEvent(Event& event) const override {
        if (event.kind != EventKind::CardDiscardedFromHand) {
            return {};
        }
        return std::make_shared<TunnelReactOption>();
    }

    int victoryPoints() const override {
        return 2;
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Tunnel,
            Card::Victory,
            NoHints,
            Cost{3}
        };
    }
};

