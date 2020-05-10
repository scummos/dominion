#pragma once

#include "turn.h"
#include "event_helpers.h"

struct CardOptionTrader : public CardOption {
    Card* trash = nullptr;
};

struct ReactOptionTrader : public EventReactOption {
    ReactOptionTrader(CardId originalCard, Deck* deck, Event& event)
        : EventReactOption(ReactKind::TraderReaction)
        , m_gainCard(originalCard)
        , m_deck(deck)
        , m_event(event) { }

    void defaultAction() override {
        if (m_accepted) {
            m_deck->gainFromSupply(CardId::Silver);
        }
    }

    CardId card() const {
        return m_gainCard;
    }

    void accept() {
        m_event.ignored = true;
        m_accepted = true;
    }

private:
    CardId m_gainCard;
    bool m_accepted = false;
    Deck* m_deck;
    Event& m_event;
};

class Trader: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionTrader*>(option_);

        auto cost = turn->cardCost(option->trash->id()).gold();

        turn->trashFromHand(option->trash);
        for (int i = 0; i < cost; i++) {
            turn->deck->gainFromSupply(CardId::Silver);
        }
    }

    std::shared_ptr<EventReactOption> reactToEvent(Event& event, Deck* playerDeck, Areas area) const override {
        if (area != Areas::Hand) {
            return {};
        }
        auto gained = cardGainedBySelf(event);
        if (!gained || gained == this) {
            return {};
        }

        if (gained->id() == CardId::Silver) {
            return {};
        }

        event.ignored = true; // original gain event doesn't happen, ReactOptionTrader gains either card
        return std::make_shared<ReactOptionTrader>(gained->id(), playerDeck, event);
    }

protected:
    friend class Supply;
    Trader() {
        m_info = {
            CardId::Trader,
            Card::Action | Card::Reaction,
            Choice,
            Cost{4}
        };
    }
};
