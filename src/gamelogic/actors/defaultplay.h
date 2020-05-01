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

    switch (card.card->id()) {
        case CardId::Mine: {
            auto hand = turn->currentHand();
            // It doesn't actually matter if you prefer copper or silver, leads to the same winrate.
            auto* treasure = worstTreasure(hand);
            if (!treasure) {
                return false;
            }
            auto better = upgradeTreasure(treasure->id());
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

        case CardId::Oasis: {
            CardOptionOasis opt;
            opt.discard = [](Hand const& hand) -> Card* {
                return leastUsefulCardInHand(hand);
            };
            card.playAction(&opt);
            return true;
        }

        case CardId::Chapel: {
            CardOptionChapel opt;
            opt.trash = garbageCards(turn->currentHandCards());
            if (opt.trash.size() > 4) {
                opt.trash.erase(opt.trash.begin()+4, opt.trash.end());
            }
            card.playAction(&opt);
            return true;
        }
        default: break;
    }
    std::cerr << "Warning: cannot default-play card " << card.card->name() << std::endl;
    return false;
}

bool defaultPlayTreasure(Turn* /*turn*/, ActiveCard& card) {
    auto hints = card.card->hints();
    if (!(hints & Card::Choice)) {
        card.playTreasure();
        return true;
    }

    switch (card.card->id()) {
        case CardId::IllGottenGains: {
            CardOptionIllGottenGains opt;
            opt.wantCopper = true;
            card.playTreasure(&opt);
            return true;
        }
        default: break;
    }

    return false;
}

void playAllTreasures(Turn* turn, Hand& hand) {
    for (auto& hcard: hand.cards) {
        if (hcard.card->hasType(Card::Treasure)) {
            defaultPlayTreasure(turn, hcard);
        }
    }
}
