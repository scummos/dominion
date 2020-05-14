#pragma once

#include "turn.h"
#include "reaction.h"

struct CardOptionSwindler : public CardOption {
    CardTransformFunc swap = identity<CardId>;
};

struct SwindlerAttack : public AttackReactOption {
    SwindlerAttack(Deck* deck, CardTransformFunc const& f) : AttackReactOption(ReactKind::SwindlerAttack, deck), m_swap(f) {}

    void defaultAction() override {
        auto uncovered = m_deck->uncoverCards(1);
        if (uncovered == 0) {
            // player might have 0 cards in draw/discard pile
            return;
        }
        auto* card = m_deck->constUncoveredDrawPile().cards().front();

        auto const newCard = m_swap(card->id());
        auto const info = m_deck->supply()->pileInfo(newCard);
        if (info.cost != card->cost()) {
            throw InvalidPlayError{"Cost of replacement card must be equal to cost of trashed card for Swindler"};
        }

        m_deck->trash(card, Areas::UncoveredDrawPile);
        m_deck->gainFromSupply(newCard);
    }

private:
    CardTransformFunc m_swap;
};

class Swindler: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->addMoney(2);

        auto* option = static_cast<CardOptionSwindler*>(option_);
        auto attack = [option](Deck* enemy) {
            return std::make_shared<SwindlerAttack>(enemy, option->swap);
        };
        turn->attackEachEnemy(attack);
    }

protected:
    friend class Supply;
    Swindler() {
        m_info = {
            CardId::Swindler,
            Card::Action,
            Choice,
            Cost{3}
        };
    }
};
