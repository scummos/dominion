#pragma once

#include "turn.h"

struct CardOptionRebuild : public CardOption {
    CardId namedCard = CardId::Province;
    std::vector<CardId> gainPrioList = {CardId::Province, CardId::Duchy, CardId::Estate};
};

class Rebuild: public Card {
public:
    void playAction(TurnInternal* turn, CardOption* option_) override {
        auto* option = static_cast<CardOptionRebuild*>(option_);
        auto const* deck = turn->deck;

        bool found = false;
        while (!found) {
            auto n = turn->deck->uncoverCards(1);
            if (n == 0) {
                // all cards uncovered, nothing found to Rebuild
                return;
            }
            auto* card = deck->uncoveredDrawPile().cards().back();
            if (card->basicInfo().id == option->namedCard) {
                // "... until [...] a card you did not name."
                continue;
            }
            if (!card->hasType(Victory)) {
                continue;
            }

            found = true;
            auto newCost = card->cost().gold() + 3;
            for (auto const gain: option->gainPrioList) {
                auto cost = turn->cardCost(gain);
                if (cost.valid() && newCost <= cost.gold()) {
                    turn->deck->gainFromSupply(gain);
                    turn->deck->trash(card, Areas::UncoveredDrawPile);
                    break;
                }
            }
        }

        turn->deck->moveAllCards(Areas::UncoveredDrawPile, Areas::DiscardPile);
    }

protected:
    friend class Supply;
    Rebuild() {
        m_info = {
            CardId::Rebuild,
            Card::Action,
            Choice | Chain,
            Cost{5}
        };
    }
};

