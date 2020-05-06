#pragma once

#include "turn.h"

struct CardOptionCartographer : public CardOption {
    // Return the cards you want to keep in the order you want to keep them.
    // First entry goes gets put back first (so, ends up at the bottom).
    using Func = std::function<Cards(const Cards&)>;
    Func discard;
};

class Cartographer: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->addActions(1);
        turn->draw(1);

        auto* option = static_cast<CardOptionCartographer*>(option_);
        if (!option || !option->discard) {
            return;
        }

        turn->deck->uncoverCards(4);
        auto remain = option->discard(static_cast<const Deck*>(turn->deck)->uncoveredDrawPile().cards());

        for (auto* card: remain) {
            turn->deck->moveCard(card, Areas::UncoveredDrawPile, Areas::DrawPile);
        }
        turn->deck->moveAllCards(Areas::UncoveredDrawPile, Areas::DiscardPile);
    }

protected:
    friend class Supply;
    Cartographer() {
        m_info = {
            CardId::Cartographer,
            Card::Action,
            Choice | Cantrip,
            Cost{5}
        };
    }
};

