#pragma once

#include "turn.h"

struct CardOptionThroneRoom : public CardOption {
    /// Action card to play twice.
    Card* card = nullptr;
    /// Card option for the first play of the selected card.
    CardOption* childOption1 = nullptr;
    /// Card option for the second play of the selected card.
    CardOption* childOption2 = nullptr;
};

class ThroneRoom : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionThroneRoom*>(option_);

        if (!option->card) {
            // valid move ("You may...")
            return;
        }

        if (!option->card->hasType(Card::Action)) {
            throw InvalidCardUsage{"Throne Room must be used on an action card."};
        }

        option->card->playAction(turn, option->childOption1);
        option->card->playAction(turn, option->childOption2);

        // The selected card is now in play as well. This is necessary because we used the
        // non-encapsulated API from the card (instead of from the turn) above in order
        // to be able to play the card twice.
        turn->deck->moveCard(option->card, Areas::Hand, Areas::InPlay);
    }

    virtual BasicInfo basicInfo() const override {
        return {
            CardId::ThroneRoom,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};



