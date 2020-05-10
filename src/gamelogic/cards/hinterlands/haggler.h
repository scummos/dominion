#pragma once

#include "turn.h"
#include "event_helpers.h"
#include "event.h"

struct ReactOptionHaggler : public EventReactOption {
    ReactOptionHaggler(Cost cost, Deck* deck) : EventReactOption(ReactKind::HagglerReaction), m_cost(cost), m_deck(deck) {}

    void accept(CardId card) {
        auto const info = m_deck->supply()->pileInfo(card);
        if (info.types & Card::Victory) {
            throw InvalidPlayError{"Haggler cannot gain Victory cards"};
        }
        if (info.cost.gold() >= m_cost.gold()) {
            throw InvalidPlayError{"Haggler requires price of gained card to be less than price of bought card"};
        }
        m_done = m_deck->gainFromSupply(card, Areas::DiscardPile, GainReason::Gain);
    }

    void defaultAction() override {
        // Gaining a card is not optional, so gain a Copper.
        if (m_done || m_cost.gold() == 0) {
            return;
        }
        accept(CardId::Copper);
    }

    Cost cost() const {
        return m_cost;
    }

private:
    bool m_done = false;
    Cost m_cost;
    Deck* m_deck;
};

class Haggler: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* /*option*/) override {
        turn->addMoney(2);
    }

    EventReactOption::Ptr reactToEvent(Event& event, Deck* playerDeck, Areas scope) const override {
        if (scope != Areas::InPlay) {
            return {};
        }
        auto* bought = cardBoughtBySelf(event);
        if (!bought) {
            return {};
        }
        return std::make_shared<ReactOptionHaggler>(bought->cost(), playerDeck);
    }

protected:
    friend class Supply;
    Haggler() {
        m_info = {
            CardId::Haggler,
            Card::Action,
            VirtualMoney,
            Cost{5}
        };
    }
};


