#pragma once

#include "turn.h"
#include "event_helpers.h"

struct CardOptionIllGottenGains : public CardOption {
    bool wantCopper = false;
};

class IllGottenGains: public Card {
public:
    void playTreasure(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionIllGottenGains*>(option_);
        turn->addMoney(1);

        if (option->wantCopper) {
            turn->deck->gainFromSupply(CardId::Copper, Areas::Hand);
        }
    }

    virtual CardTraits traits() const override {
        return CardTraits{
            1
        };
    };

    std::shared_ptr<EventReactOption> reactToEvent(Event& event, Deck* playerDeck) const override {
        if (!eventGainsThisCard(event, this)) {
            return {};
        }

        for (auto* enemy: playerDeck->enemies()) {
            enemy->gainFromSupply(CardId::Curse, Areas::DiscardPile);
        }
        return {};
    }

protected:
    friend class Supply;
    IllGottenGains() {
        m_info = {
            CardId::IllGottenGains,
            Card::Treasure,
            Choice,
            Cost{5}
        };
    }
};


