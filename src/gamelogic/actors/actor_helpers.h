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

void defaultPlay(Turn* turn, ActiveCard& card) {
    if (turn->currentActions() <= 0) {
        return;
    }

    if (!(card.card->hints() & Card::Choice)) {
        card.playAction();
    }
    else {
        std::cerr << "Warning: cannot default-play card" << card.card->name() << std::endl;
    }
}

Cards garbageCards(Cards const& hand) {
    Cards ret;
    for (auto const& card: hand) {
        auto const info = card->basicInfo();
        if (info.id == CardId::Copper || info.id == CardId::Estate) {
            ret.push_back(card);
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

void playAllTreasures(Hand& hand) {
    for (auto& hcard: hand.cards) {
        if (hcard.card->hasType(Card::Treasure)) {
            hcard.playTreasure();
        }
    }
}

void defaultReact(EventReactOption& option) {
    switch (option.kind()) {
    case ReactKind::TorturerAttack: {
        auto& opt = static_cast<TorturerAttackReactOption&>(option);
        auto cards = opt.cards();

        // Do we even have any cards? If no, chose discard, we can't discard anything anyways.
        if (cards.empty()) {
            opt.chooseDiscard(cards);
            break;
        }

        // Do we have at least 2 garbage cards anyways? Discard them.
        auto const& garbage = garbageCards(cards);
        if (garbage.size() >= 2) {
            opt.chooseDiscard({garbage[0], garbage[1]});
            break;
        }

        // Otherwise, maybe better take the curse.
        opt.chooseCurse();
        break;
    }

    case ReactKind::IgnoreAttackReaction: {
        auto& opt = static_cast<IgnoreAttackReactOption&>(option);
        // always want to do this
        opt.accept();
        break;
    }

    case ReactKind::NoChoiceAttack:
        // nothing we can do about this
        break;

    case ReactKind::DiscardAttack: {
        // TODO
    }

    default:
        break;
    }
}
