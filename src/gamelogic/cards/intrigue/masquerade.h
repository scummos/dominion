#pragma once

#include "turn.h"
#include "event_helpers.h"

struct CardOptionMasquerade : public CardOption {
    // Here you only choose what to trash. The passing on of cards is handled
    // using an event (react to MasqueradeReactOption to choose the card).
    DiscardFunc trash;
};

struct MasqueradeReactOption : public EventReactOption {
    MasqueradeReactOption(Deck* d) : EventReactOption(ReactKind::MasqueradeReaction), fromPlayer(d) { };

private:
    Deck* fromPlayer;

public:
    void setCard(Card* card) {
        passOn = card;
    }

    void defaultAction() override {
        if (!passOn) {
            if (fromPlayer->constHand().count() == 0) {
                return;
            }
            passOn = fromPlayer->constHand().cards().front();
        }
    };

    Card* passOn = nullptr;
};

class Masquerade: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        turn->draw(2);

        // We have to do the passing of cards in two steps: first let each player choose,
        // then pass the cards. Otherwise, we'll get confused and make it possible to pass on
        // cards that were just received.
        auto players = turn->deck->enemies();
        players.push_back(turn->deck);
        std::vector<Card*> pass;
        for (int i = 0; i < players.size(); i++) {
            auto reactOpt = std::make_shared<MasqueradeReactOption>(players[i]);
            players[i]->choiceGiven(reactOpt);
            pass.push_back(reactOpt->passOn);
        }

        for (int i = 0; i < players.size(); i++) {
            auto const next = (i + 1) % players.size();
            if (!pass[i]) {
                // can happen if a player should have 0 hand cards
                continue;
            }
            players[i]->moveCard(pass[i], Areas::Hand, Areas::Hand, players[next]);
        }

        auto* option = static_cast<CardOptionMasquerade*>(option_);
        auto trash = option->trash(turn->turn->currentHandCards());
        if (!trash.empty()) {
            turn->trashFromHand(trash.front());
        }
    }

protected:
    friend class Supply;
    Masquerade() {
        m_info = {
            CardId::Masquerade,
            Card::Action,
            Draw | Choice,
            Cost{3}
        };
    }
};
