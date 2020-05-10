#pragma once

#include "turn.h"
#include "reaction.h"

struct MargraveAttackReactOption : public AttackReactOption {
    MargraveAttackReactOption(Deck* deck) : AttackReactOption(ReactKind::MargraveAttack, deck) {}

    virtual void defaultAction() override {
        chooseDiscard([](Cards const& c) -> Cards { return c; });
        m_done = true;
    }

    void chooseDiscard(DiscardFunc const& discard) {
        if (m_done) {
            return;
        }
        m_deck->drawCards(1);
        for (auto* card: discard(m_deck->constHand().cards())) {
            if (m_deck->constHand().count() <= 3) {
                break;
            }
            m_deck->discardFromHand(card);
        }

        if (m_deck->constHand().count() > 3) {
            defaultAction();
        }
        m_done = true;
    }

private:
    bool m_done = false;
};


class Margrave: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(3);
        turn->addBuys(1);

        auto attack = [](Deck* enemy) {
            return std::make_shared<MargraveAttackReactOption>(enemy);
        };
        turn->attackEachEnemy(attack);
    }

protected:
    friend class Supply;
    Margrave() {
        m_info = {
            CardId::Margrave,
            Card::Action,
            TerminalDraw | Attack,
            Cost{5}
        };
    }
};

