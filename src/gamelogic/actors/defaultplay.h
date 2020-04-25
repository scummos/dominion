#pragma once

#include "turn.h"
#include "mine.h"
#include "actor_helpers.h"

void defaultPlay(Turn* turn, ActiveCard& card) {
    if (turn->currentActions() <= 0 || !card.card->hasType(Card::Action)) {
        return;
    }

    auto hints = card.card->hints();
    if (!(hints & Card::Choice)) {
        card.playAction();
        return;
    }

    switch (card.card->basicInfo().id) {
        case CardId::Mine: {
            auto hand = turn->currentHand();
            // It doesn't actually matter if you prefer copper or silver, leads to the same winrate.
            auto* treasure = worstTreasure(hand);
            if (!treasure) {
                return;
            }
            auto better = upgradeTreasure(treasure->basicInfo().id);
            if (better == CardId::Invalid) {
                return;
            }

            CardOptionMine opt;
            opt.trash = treasure;
            opt.gain = better;
            card.playAction(&opt);
            return;
        }
        default: break;
    }
    std::cerr << "Warning: cannot default-play card " << card.card->name() << std::endl;
}
