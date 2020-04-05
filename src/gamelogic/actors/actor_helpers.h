#pragma once

#include "turn.h"

void defaultVillageDraw(Turn* turn, int wantActionsRemain = 0) {
    while (turn->currentActions() > wantActionsRemain) {
        auto hand = turn->currentHand();
        // Play all cards that give actions.
        for (auto& card: hand.cards) {
            auto hints = card.card->hints();
            if ((hints & Card::Chain) && !(hints & Card::Choice)) {
                card.playAction();
            }
        }

        // Play all draw cards.
        hand = turn->currentHand();
        auto draw = hand.findCards(Card::Draw);
        if (draw.empty()) {
            // Nothing else to do, stop.
            break;
        }
        draw.front().playAction();
    }
}

Cards garbageCards(Hand const& hand) {
    Cards ret;
    for (auto const& hcard: hand.cards) {
        auto const info = hcard.card->basicInfo();
        if (info.id == CardId::Copper || info.id == CardId::Estate) {
            ret.push_back(hcard.card);
        }
    }
    return ret;
}

int plainTreasureInHand(Hand const& hand) {
    int ret = 0;
    for (auto const& hcard: hand.cards) {
        auto id = hcard.card->basicInfo().id;
        switch (id) {
            case CardId::Copper: ret += 1; break;
            case CardId::Silver: ret += 2; break;
            case CardId::Gold:   ret += 3; break;
            default: break;
        };
    }
    return ret;
}
