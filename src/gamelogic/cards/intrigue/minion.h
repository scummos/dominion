#pragma once

#include "turn.h"
#include "reaction.h"

struct CardOptionMinion : public CardOption {
    enum Choice {
        TwoMoney = 1,
        DrawNewHand = 2
    };
    Choice choice = TwoMoney;
};

struct MinionAttackReactOption : public AttackReactOption {
    MinionAttackReactOption(Deck* deck) : AttackReactOption(ReactKind::MinionAttack, deck) {}

    virtual void defaultAction() override {
        auto cards = m_deck->constHand().cards();
        for (auto* card: cards) {
            m_deck->discardFromHand(card);
        }
        m_deck->drawCards(4);
    }
};

class Minion: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->addActions(1);

        auto* opt = static_cast<CardOptionMinion*>(option_);
        switch (opt->choice) {
            case CardOptionMinion::TwoMoney:
                turn->addMoney(2);
                break;

            case CardOptionMinion::DrawNewHand: {
                auto handCards = turn->deck->constHand().cards();
                for (auto* card: handCards) {
                    turn->discardFromHand(card);
                }
                turn->draw(4);

                auto attack = [](Deck* enemy) {
                    return std::make_shared<MinionAttackReactOption>(enemy);
                };
                turn->attackEachEnemy(attack);

                break;
            }

            default:
                throw InvalidPlayError{"Invalid choice for Minion"};
        }
    }

protected:
    friend class Supply;
    Minion() {
        m_info = {
            CardId::Minion,
            Card::Action,
            Choice | Attack,
            Cost{5}
        };
    }
};
