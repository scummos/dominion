#pragma once

#include "turn.h"

struct CardOptionNobles : public CardOption {
    enum Choice {
        ThreeCards,
        TwoActions
    };
    Choice choice = ThreeCards;
};

class Nobles : public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionNobles*>(option_);
        switch (option->choice) {
            case CardOptionNobles::ThreeCards:
                turn->draw(3);
                break;

            case CardOptionNobles::TwoActions:
                turn->addActions(2);
                break;

            default:
                throw InvalidPlayError{"Invalid choice for Nobles"};
        }
    }

    int victoryPoints(Deck const*) const override {
        return 2;
    }

protected:
    friend class Supply;
    Nobles() {
        m_info = {
            CardId::Nobles,
            Card::Action | Card::Victory,
            NoHints,
            6
        };
    }
};
