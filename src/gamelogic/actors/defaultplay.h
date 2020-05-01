#pragma once

#include "turn.h"
#include "mine.h"
#include "mandarin.h"
#include "rebuild.h"
#include "actor_helpers.h"

bool defaultPlay(Turn* turn, ActiveCard& card) {
    if (turn->currentActions() <= 0 || !card.card->hasType(Card::Action)) {
        return false;
    }

    auto hints = card.card->hints();
    if (!(hints & Card::Choice)) {
        card.playAction();
        return true;
    }

    switch (card.card->basicInfo().id) {
        case CardId::Mine: {
            auto hand = turn->currentHand();
            // It doesn't actually matter if you prefer copper or silver, leads to the same winrate.
            auto* treasure = worstTreasure(hand);
            if (!treasure) {
                return false;
            }
            auto better = upgradeTreasure(treasure->basicInfo().id);
            if (better == CardId::Invalid) {
                return false;
            }

            CardOptionMine opt;
            opt.trash = treasure;
            opt.gain = better;
            card.playAction(&opt);
            return true;
        }

        case CardId::Mandarin: {
            CardOptionMandarin opt;
            auto hand = turn->currentHand();
            opt.putBack = leastUsefulCardInHand(hand, card.card);
            card.playAction(&opt);
            return true;
        }

        case CardId::Rebuild: {
            CardOptionRebuild opt;
            opt.namedCard = CardId::Province;
            card.playAction(&opt);
            return true;
        }
        default: break;
    }
    std::cerr << "Warning: cannot default-play card " << card.card->name() << std::endl;
    return false;
}
