#pragma once

#include "turn.h"
#include "reaction.h"

struct TorturerAttackReactOption : public AttackReactOption {
    TorturerAttackReactOption(Deck* deck) : AttackReactOption(ReactKind::TorturerAttack, deck) { }

    Cards cards() {
        return static_cast<const Deck*>(m_deck)->hand().cards();
    }

    void chooseCurse() {
        if (m_choiceMade) {
            return;
        }
        m_deck->gainFromSupply(CardId::Curse, Areas::Hand);
        m_choiceMade = true;
    }

    void chooseDiscard(std::vector<Card*> cards) {
        if (m_choiceMade) {
            return;
        }
        if (cards.size() != std::min<int>(2, static_cast<const Deck*>(m_deck)->hand().cards().size())) {
            throw InvalidPlayError{"Must select exactly 2 cards to discard"};
        }
        for (auto* card: cards) {
            m_deck->discardFromHand(card);
        }
        m_choiceMade = true;
    }

    void defaultAction() override {
        if (m_choiceMade) {
            return;
        }
        chooseCurse();
    };

protected:
    bool m_choiceMade = false;
};

class Torturer: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->draw(3);

        auto opt = std::make_shared<TorturerAttackReactOption>(turn->deck);
        turn->attackEachEnemy(opt);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::Torturer,
            Card::Action,
            TerminalDraw | Attack,
            Cost{5}
        };
    }
};

