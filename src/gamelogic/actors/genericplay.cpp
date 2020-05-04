#include "genericplay.h"

#include "remodel.h"
#include "embassy.h"
#include "trader.h"

#define info(x)

namespace {
    template<typename T>
    std::vector<T> anyToList(std::any any) {
        using V = std::vector<T>;
        using VA = std::vector<std::any>;
        if (any.type() == typeid(VA)) {
            auto temp = std::any_cast<VA>(any);
            V ret(temp.size());
            std::transform(temp.begin(), temp.end(), ret.begin(), [](std::any const& elem) { return std::any_cast<T>(elem); });
            return ret;
        }
        return V{std::any_cast<T>(any)};
    }

    Cards cardsByValue(Hand const& hand) {
        Cards ret;
        for (auto& c: hand.cards) {
            ret.push_back(c.card);
        }
        std::sort(ret.begin(), ret.end(), [](Card* c1, Card* c2) { return c1->cost().gold() < c2->cost().gold(); });
        return ret;
    }

    Cards findInHand(Hand const& hand, std::vector<CardId> cards, int max = -1) {
        Cards ret;
        for (auto const& choice: cards) {
            for (auto const& card: hand.findCards(choice)) {
                ret.push_back(card.card);
            }
        }
        if (max != -1 && ret.size() > max) {
            ret.erase(ret.begin()+max, ret.end());
        }
        return ret;
    }

    Cards findInHandExact(Hand const& hand, std::vector<CardId> cards, int count) {
        auto base = findInHand(hand, cards, count);
        auto missing = count - base.size();
        if (missing == 0) {
            return base;
        }
        auto extra = cardsByValue(hand);
        for (auto* card: extra) {
            if (std::find(base.begin(), base.end(), card) != base.end()) {
                continue;
            }
            base.push_back(card);
            if (base.size() == count) {
                break;
            }
        }
        return base;
    }

    Card* firstChoice(Hand const& hand, std::vector<CardId> choices) {
        for (auto const& choice: choices) {
            if (hand.hasCard(choice)) {
                return hand.findCards(choice).back().card;
            }
        }
        return nullptr;
    }

    using CardPair = std::pair<CardId, CardId>;
}

bool genericPlay(Turn* turn, ActiveCard card, std::any opt)
{
    if (turn->currentActions() <= 0) {
        return false;
    }
    if (!opt.has_value() || !(card.card->hints() & Card::Choice)) {
        return defaultPlay(turn, card);
    }

    auto hand = turn->currentHand();
    hand.ignore(card.card);
    switch (card.card->id()) {
        case CardId::Remodel: {
            auto const& choices = anyToList<CardPair>(opt);
            for (auto const& choice: choices) {
                if (hand.hasCard(choice.first)) {
                    CardOptionRemodel opt;
                    opt.trash = hand.findCards(choice.first).back().card;
                    opt.gain = choice.second;
                    info(std::cerr << "Remodel: " << cardName(choice.first) << " -> " << cardName(choice.second) << std::endl);
                    card.playAction(&opt);
                    return true;
                }
            }
            // Otherwise, we do not play Remodel, as we have nothing to use it on. That's fine.
            return false;
        }

        case CardId::Chapel: {
            auto const& choices = anyToList<CardId>(opt);
            auto trash = findInHand(hand, choices, 4);

            if (trash.size() > 0) {
                CardOptionChapel opt;
                opt.trash = trash;
                card.playAction(&opt);
                return true;
            }
            return false;
        }

        case CardId::Embassy: {
            auto chooseDiscard = [opt](Hand const& hand) {
                auto const& choices = anyToList<CardId>(opt);
                return findInHandExact(hand, choices, 3);
            };
            CardOptionEmbassy opt;
            opt.discard = chooseDiscard;
            card.playAction(&opt);
            return true;
        }

        case CardId::Trader: {
            auto const& choices = anyToList<CardId>(opt);
            auto* choice = firstChoice(turn->currentHand(), choices);
            if (!choice) {
                return false;
            }
            CardOptionTrader opt;
            opt.trash = choice;
            card.playAction(&opt);
            return true;
        }
        default: break;
    }

    std::cerr << "failed: " << card.card->name() << std::endl;
    throw InvalidPlayError{"Cannot generic-play this action"};
}

void genericPlayTreasure(Turn* turn, ActiveCard card, std::any opt)
{
    if (!opt.has_value() || !(card.card->hints() & Card::Choice)) {
        defaultPlayTreasure(turn, card);
        return;
    }

    switch (card.card->id()) {
        case CardId::IllGottenGains: {
            auto choice = std::any_cast<bool>(opt);
            CardOptionIllGottenGains opt;
            opt.wantCopper = choice;
            card.playTreasure(&opt);
            return;
        }
        default: break;
    }
    std::cerr << "failed: " << card.card->name() << std::endl;
    throw InvalidPlayError{"Cannot generic-play this treasure"};
}


