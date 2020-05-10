#pragma once

#include "turn.h"
#include "reaction.h"

struct MilitiaAttackReactOption : public AttackReactOption {
    MilitiaAttackReactOption(Deck* deck) : AttackReactOption(ReactKind::MilitiaAttack, deck) {}

    virtual void defaultAction() override {
        chooseDiscard([](Cards const& c) -> Cards { return c; });
        m_done = true;
    }

    void chooseDiscard(DiscardFunc const& discard) {
        if (m_done) {
            return;
        }
        discardDownTo(m_deck, 3, discard);
        m_done = true;
    }

private:
    bool m_done = false;
};


class Militia: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addMoney(2);

        auto attack = [](Deck* enemy) {
            return std::make_shared<MilitiaAttackReactOption>(enemy);
        };
        turn->attackEachEnemy(attack);
    }

protected:
    friend class Supply;
    Militia() {
        m_info = {
            CardId::Militia,
            Card::Action,
            VirtualMoney | Attack,
            Cost{4}
        };
    }
};


