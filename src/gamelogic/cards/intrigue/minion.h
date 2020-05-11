#pragma once

#include "turn.h"
#include "reaction.h"

struct CardOptionMinion : public CardOption {
    enum Choice {
        TwoMoney,
        DrawNewHand
    };
    Choice choice = TwoMoney;
};

struct MinionAttackReactOption : public AttackReactOption {
    MinionAttackReactOption(Deck* deck) : AttackReactOption(ReactKind::MinionAttack, deck) {}

    virtual void defaultAction() override {
        for (auto* card: m_deck->constHand().cards()) {
            m_deck->discardFromHand(card);
        }
        m_deck->drawCards(4);
    }
};

class Minion: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* opt = static_cast<CardOptionMinion*>(option_);
        switch (opt->choice) {
            case CardOptionMinion::TwoMoney:
                turn->addMoney(2);
                break;

            case CardOptionMinion::DrawNewHand: {
                for (auto* card: turn->deck->constHand().cards()) {
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
