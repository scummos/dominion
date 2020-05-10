#pragma once

#include "turn.h"
#include "event_helpers.h"

struct CardOptionEmbassy : public CardOption {
    std::function<std::vector<Card*>(Hand const&)> discard;
};

class Embassy: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->draw(5);

        auto* option = static_cast<CardOptionEmbassy*>(option_);
        auto hand = turn->turn->currentHand();
        auto discard = option->discard(hand);
        if (!(discard.size() == 3 || (discard.size() < 3 && discard.size() == hand.cards.size()))) {
            throw InvalidPlayError{"Embassy discards exactly 3 cards"};
        }
        for (auto* card: discard) {
            turn->discardFromHand(card);
        }
    }

    std::shared_ptr<EventReactOption> reactToEvent(Event& event, Deck* playerDeck, Areas) const override {
        if (!eventGainsThisCard(event, this)) {
            return {};
        }

        for (auto* enemy: playerDeck->enemies()) {
            enemy->gainFromSupply(CardId::Silver, Areas::DiscardPile);
        }
        return {};
    }

protected:
    friend class Supply;
    Embassy() {
        m_info = {
            CardId::Embassy,
            Card::Action,
            Draw | Choice,
            Cost{5}
        };
    }
};



