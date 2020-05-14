#pragma once

#include "turn.h"

struct CardOptionThroneRoom : public CardOption {
    /// Action card to play twice.
    Card* card = nullptr;

    /// Function to play the selected card. This function must play @p card
    /// exactly once by calling card.playAction(), passing in the relevant options.
    /// If the card you selected to play twice has no options, the simplest implementation
    /// of this is just [](auto card) { card.playAction(); }.
    std::function<void(ActiveCard& card)> playFunc;
};

class ThroneRoom : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionThroneRoom*>(option_);

        if (!option || !option->card || !option->playFunc) {
            // valid move ("You may...")
            return;
        }

        if (!option->card->hasType(Card::Action)) {
            throw InvalidPlayError{"Throne Room must be used on an action card."};
        }

        if (option->card == this) {
            throw InvalidPlayError{"Throne Room cannot play itself twice."};
        }

        // The selected card is now in play as well. This is necessary because we used the
        // non-encapsulated API from the card (instead of from the turn) below in order
        // to be able to play the card twice.
        turn->deck->moveCard(option->card, Areas::Hand, Areas::InPlay);

        bool wasCalled;
        auto playSelectedCard = [&wasCalled, option, turn](CardOption* opt) {
            if (wasCalled) {
                throw InvalidPlayError{"Throne room must play the selected card not more than twice."};
            }
            // This doesn't use the turn API, so it does not use up an action,
            // nor does it move the card anywhere.
            option->card->playAction(turn, opt);
            wasCalled = true;
        };

        ActiveCard active;
        active.card = option->card;
        active.requiresActionToPlay = false;
        active.playActionImpl = playSelectedCard;

        // Play it for the first time.
        wasCalled = false;
        option->playFunc(active);
        if (!wasCalled) {
            throw InvalidPlayError{"Throne room must play the selected card not less than twice."};
        }

        // Play it for the second time.
        wasCalled = false;
        option->playFunc(active);
        if (!wasCalled) {
            throw InvalidPlayError{"Throne room must play the selected card not less than twice."};
        }
    }

protected:
    friend class Supply;
    ThroneRoom() {
        m_info = {
            CardId::ThroneRoom,
            Card::Action,
            Choice,
            Cost{4}
        };
    }
};
