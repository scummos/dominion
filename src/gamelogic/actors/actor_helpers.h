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

        // remove choice cards
        draw.erase(std::remove_if(draw.begin(), draw.end(), [](ActiveCard const& c) {
            return c.card->hints() & Card::Choice;
        }), draw.end());

        if (draw.empty()) {
            // nothing else to do, stop.
            break;
        }

        // select highest-value card
        std::sort(draw.begin(), draw.end(), [](ActiveCard const& c1, ActiveCard const& c2) {
            return c1.card->cost().gold() < c2.card->cost().gold();
        });
        draw.back().playAction();
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

int usefulness(Card* card) {
    switch (card->basicInfo().id) {
        case CardId::Curse:
            return -2;

        case CardId::Province:
        case CardId::Duchy:
        case CardId::Estate:
            return -1;

        default: return card->cost().gold();
    }
}

Card* leastUsefulCardInHand(Hand const& hand, Card* exclude = nullptr) {
    if (hand.cards.empty()) {
        return nullptr;
    }
    auto worst = std::min_element(hand.cards.begin(), hand.cards.end(), [exclude](ActiveCard const& c1, ActiveCard const& c2) {
        return c1.card == exclude ? false : c2.card == exclude ? true : usefulness(c1.card) < usefulness(c2.card);
    })->card;
    return worst == exclude ? nullptr : worst;
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

enum class CardSortOrder {
    LeastExpensive,
    MostExpensive
};

Card* worstTreasure(Hand& hand, CardSortOrder order = CardSortOrder::LeastExpensive) {
    Card* worst = nullptr;
    int cost = order == CardSortOrder::LeastExpensive ? 1000 : -1000;
    for (auto& hcard: hand.cards) {
        if (!hcard.card->hasType(Card::Treasure)) {
            continue;
        }

        auto const thisCost = hcard.card->basicInfo().cost.gold();
        if (order == CardSortOrder::LeastExpensive ? thisCost <= cost : thisCost >= cost) {
            worst = hcard.card;
            cost = thisCost;
        }
    }
    return worst;
}

CardId upgradeTreasure(CardId id) {
    switch (id) {
        case CardId::Copper: return CardId::Silver;
        case CardId::Silver: return CardId::Gold;
        default: break;
    }
    return CardId::Invalid;
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
