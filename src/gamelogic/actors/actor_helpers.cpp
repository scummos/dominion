#include "actor_helpers.h"
#include "trader.h"
#include "margrave.h"
#include "genericplay.h"

void defaultVillageDraw(Turn* turn, int wantActionsRemain)
{
    while (turn->currentActions() > wantActionsRemain) {
        // Play all cards that give actions.
        for (int i = 0; i < turn->currentHandSize(); i++) {
            auto const& hand = turn->currentHand();
            auto* card = hand.cards.at(i);
            auto activeCard = hand.activeCard(card);
            auto hints = card->hints();
            if ((hints & Card::Chain) && !(hints & Card::Choice)) {
                activeCard.playAction();
            }
        }

        // Play one draw card.
        auto const& hand2 = turn->currentHand();
        auto draw = hand2.findCards(Card::Draw);

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

Cards garbageCards(const Cards& hand)
{
    Cards ret;
    for (auto const& card: hand) {
        auto const info = card->basicInfo();
        if (info.id == CardId::Copper || info.id == CardId::Estate) {
            ret.push_back(card);
        }
    }
    return ret;
}

int usefulness(Card* card)
{
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

Card* leastUsefulCardInHand(Hand const& hand, Card* exclude) {
    if (hand.cards.empty()) {
        return nullptr;
    }
    auto worst = *std::min_element(hand.cards.begin(), hand.cards.end(), [exclude](Card* c1, Card* c2) {
        return c1 == exclude ? false : c2 == exclude ? true : usefulness(c1) < usefulness(c2);
    });
    return worst == exclude ? nullptr : worst;
}

int plainTreasureInHand(Cards const& hand) {
    int ret = 0;
    for (auto const& card: hand) {
        ret += card->traits().treasureValue;
    }
    return ret;
}

Card* worstTreasure(Hand& hand, CardSortOrder order) {
    Card* worst = nullptr;
    int cost = order == CardSortOrder::LeastExpensive ? 1000 : -1000;
    for (auto& card: hand.cards) {
        if (!card->hasType(Card::Treasure)) {
            continue;
        }

        auto const thisCost = card->basicInfo().cost.gold();
        if (order == CardSortOrder::LeastExpensive ? thisCost <= cost : thisCost >= cost) {
            worst = card;
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

void defaultReact(Deck const* deck, EventReactOption::Ptr option) {
    using AnyVector = std::vector<std::any>;

    switch (option->kind()) {
    case ReactKind::TorturerAttack: {
        auto opt = std::static_pointer_cast<TorturerAttackReactOption>(option);
        auto cards = opt->cards();

        // Do we even have any cards? If no, chose discard, we can't discard anything anyways.
        if (cards.empty()) {
            opt->chooseDiscard(cards);
            break;
        }

        // Do we have at least 2 garbage cards anyways? Discard them.
        auto const& garbage = garbageCards(cards);
        if (garbage.size() >= 2) {
            opt->chooseDiscard({garbage[0], garbage[1]});
            break;
        }

        // Otherwise, maybe better take the curse.
        opt->chooseCurse();
        break;
    }

    case ReactKind::IgnoreAttackReaction: {
        auto opt = std::static_pointer_cast<IgnoreAttackReactOption>(option);
        // always want to do this
        opt->accept();
        break;
    }

    case ReactKind::NoChoiceAttack:
        // nothing we can do about this
        break;

    case ReactKind::TraderReaction: {
        genericReact(deck, option, AnyVector{CardId::Curse});
        break;
    }

    case ReactKind::MargraveAttack: {
        genericReact(deck, option, AnyVector{CardId::Province, CardId::Duchy, CardId::Estate, CardId::Curse, CardId::Copper});
        break;
    }

    case ReactKind::MasqueradeReaction: {
        genericReact(deck, option, AnyVector{CardId::Curse, CardId::Copper, CardId::Estate});
        break;
    }

    default:
        break;
    }
}
