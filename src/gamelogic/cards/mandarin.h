#pragma once

#include "turn.h"

struct CardOptionMandarin : public CardOption {
    Card* putBack = nullptr;
};

class Mandarin: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionMandarin*>(option_);

        if (!option->putBack && turn->countCardsInHand() > 0) {
            throw InvalidCardUsage{"Mandarin requires specifying a card to put on your draw pile."};
        }
        if (option->putBack == this) {
            throw InvalidCardUsage{"Not this one, though"};
        }

        turn->addMoney(3);
        if (option->putBack) {
            turn->deck->moveCard(option->putBack, Areas::Hand, Areas::DrawPile);
        }
    }

    EventReactOption::Ptr reactToEvent(Event& event, Deck* playerDeck) const override {
        if (event.kind != EventKind::YouGainACard) {
            return {};
        }
        auto& gainEvent = static_cast<YouGainACardEvent&>(event);
        if (gainEvent.card != this) {
            return {};
        }

        std::vector<Card*> putBack;
        for (auto* card: static_cast<const Deck*>(playerDeck)->inPlay().cards()) {
            if (!card->hasType(Card::Treasure)) {
                continue;
            }
            putBack.push_back(card);
        }

        for (auto* card: putBack) {
            playerDeck->moveCard(card, Areas::InPlay, Areas::DrawPile);
        }
        return {};
    }

protected:
    friend class Supply;
    Mandarin() {
        m_info = {
            CardId::Mandarin,
            Card::Action,
            Choice,
            Cost{5}
        };
    }
};


