#include "genericplay.h"

#include "supply.h"

#include "remodel.h"
#include "embassy.h"
#include "trader.h"
#include "cartographer.h"
#include "margrave.h"
#include "militia.h"
#include "haggler.h"

#define info(x)

namespace {
    template<typename T>
    std::vector<T> anyToList(std::any any) {
        using V = std::vector<T>;
        using VA = std::vector<std::any>;
        info(std::cerr << "anyToList " << typeid(T).name() << " from " << any.type().name() << std::endl);
        if (any.type() == typeid(VA)) {
            auto temp = std::any_cast<VA>(any);
            V ret(temp.size());
            std::transform(temp.begin(), temp.end(), ret.begin(), [](std::any const& elem) { return std::any_cast<T>(elem); });
            return ret;
        }
        return V{std::any_cast<T>(any)};
    }

    enum class TagRequirement {
        Implicit, //< "options for Chapel Curse end" returns "Curse" when asked for Trash tag
        Explicit //< the above returns null instead, need "options for Chapel trash Curse end" for it to work
    };

    std::any getTagged(std::any any, OptionTag tag, TagRequirement mode = TagRequirement::Implicit) {
        using VE = std::vector<TaggedExpr>;
        info(std::cerr << "getTagged from " << any.type().name() << std::endl);
        if (any.type() == typeid(TaggedExpr)) {
            auto expr = std::any_cast<TaggedExpr>(any);
            if (expr.tag == tag) {
                return expr.option;
            }
        }
        if (any.type() == typeid(VE)) {
            auto v = std::any_cast<VE>(any);
            auto it = std::find_if(v.begin(), v.end(), [tag](TaggedExpr const& e) {
                return e.tag == tag;
            });
            if (it != v.end()) {
                return it->option;
            }
        }
        if (mode == TagRequirement::Implicit) {
            // Just assume the whole option refers to this tag, if requested
            return any;
        }
        return std::any();
    }

    std::vector<CardId> getTaggedCardList(std::any any, OptionTag tag, TagRequirement mode = TagRequirement::Implicit) {
        return anyToList<CardId>(getTagged(any, tag, mode));
    }

    int getSelectedOption(std::any any, int defaultIfNotSpecified = 1) {
        auto tag = getTagged(any, OptionTag::ChooseOption, TagRequirement::Explicit);
        if (!tag.has_value()) {
            return defaultIfNotSpecified;
        }
        return std::any_cast<int>(tag);
    }

    auto makeDiscardFunc(std::vector<CardId> const& discard) {
        return std::function([discard](const Cards& in) {
            Cards out;
            for (auto const& card: in) {
                if (std::find(discard.begin(), discard.end(), card->id()) != discard.end()) {
                    out.push_back(card);
                }
            }
            return out;
        });
    }

    auto makeDiscardOrderFunc(std::vector<CardId> const& discard) {
        return std::function([discard](const Cards& in) {
            Cards out = in;
            std::sort(out.begin(), out.end(), [&discard](auto c1, auto c2) {
                auto discard_c1 = std::distance(discard.begin(), std::find(discard.begin(), discard.end(), c1->id()));
                auto discard_c2 = std::distance(discard.begin(), std::find(discard.begin(), discard.end(), c2->id()));
                return   discard_c1 == discard_c2 ? c1->cost().gold() > c2->cost().gold()
                       : discard_c1 < discard_c2;
            });
            return out;
        });
    }

    Cards cardsByValue(CardPile const& hand) {
        Cards ret;
        for (auto& c: hand.cards()) {
            ret.push_back(c);
        }
        std::sort(ret.begin(), ret.end(), [](Card* c1, Card* c2) { return c1->cost().gold() < c2->cost().gold(); });
        return ret;
    }

    Cards findInHand(CardPile const& hand, std::vector<CardId> cards, int max = -1) {
        Cards ret;
        for (auto const& choice: cards) {
            for (auto const& card: hand.findCards(choice)) {
                ret.push_back(card);
            }
        }
        if (max != -1 && ret.size() > max) {
            ret.erase(ret.begin()+max, ret.end());
        }
        return ret;
    }

    Cards findInHandExact(CardPile const& hand, std::vector<CardId> cards, int count) {
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

    Card* firstChoice(CardPile const& hand, std::vector<CardId> choices) {
        for (auto const& choice: choices) {
            if (hand.hasCard(choice)) {
                return hand.findCards(choice).back();
            }
        }
        return nullptr;
    }

    using CardPair = std::pair<CardId, CardId>;
}

bool genericReact(Deck const* deck, EventReactOption::Ptr reactOption, std::any opt)
{
    if (!opt.has_value()) {
        return false;
    }

    switch (reactOption->kind()) {
        case ReactKind::TraderReaction: {
            auto reactOpt = std::static_pointer_cast<ReactOptionTrader>(reactOption);
            auto trade = anyToList<CardId>(opt);
            if (std::find(trade.begin(), trade.end(), reactOpt->card()) != trade.end()) {
                reactOpt->accept();
            }
            return true;
        }

        case ReactKind::HagglerReaction: {
            auto reactOpt = std::static_pointer_cast<ReactOptionHaggler>(reactOption);
            auto gainList = anyToList<CardId>(opt);
            auto cost = reactOpt->cost();
            auto gainIt = std::find_if(gainList.begin(), gainList.end(), [cost, deck](auto card) {
                return deck->supply()->pileInfo(card).cost.gold() < cost.gold();
            });
            if (gainIt == gainList.end()) {
                // No card specfied which we could gain :(
                return false;
            }
            reactOpt->accept(*gainIt);
            return true;
        }

        case ReactKind::TorturerAttack: {
            auto reactOpt = std::static_pointer_cast<TorturerAttackReactOption>(reactOption);
            auto selected = getSelectedOption(opt, 1);
            switch (selected) {
                case 1: {
                    auto discardTypes = getTaggedCardList(opt, OptionTag::Discard, TagRequirement::Explicit);
                    auto discard = findInHandExact(deck->constHand(), discardTypes, 2);
                    reactOpt->chooseDiscard(discard);
                    break;
                }
                case 2: {
                    reactOpt->chooseCurse();
                    break;
                }
                default: throw InvalidPlayError{"Requested option for TorturerAttack not available"};
            }
            return true;
        }

        case ReactKind::MargraveAttack: {
            auto reactOpt = std::static_pointer_cast<MargraveAttackReactOption>(reactOption);
            reactOpt->chooseDiscard(makeDiscardOrderFunc(getTaggedCardList(opt, OptionTag::Discard)));
            return true;
        }

        case ReactKind::MilitiaAttack: {
            auto reactOpt = std::static_pointer_cast<MilitiaAttackReactOption>(reactOption);
            reactOpt->chooseDiscard(makeDiscardOrderFunc(getTaggedCardList(opt, OptionTag::Discard)));
            return true;
        }

        default: break;
    }
    return false;
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
            auto const& choices = getTaggedCardList(opt, OptionTag::Trash);
            auto trash = findInHand(hand.cards, choices, 4);

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
                auto const& choices = getTaggedCardList(opt, OptionTag::Discard);
                return findInHandExact(hand.cards, choices, 3);
            };
            CardOptionEmbassy opt;
            opt.discard = chooseDiscard;
            card.playAction(&opt);
            return true;
        }

        case CardId::Trader: {
            auto part = getTagged(opt, OptionTag::Trash);
            auto const& choices = anyToList<CardId>(part);
            auto* choice = firstChoice(hand.cards, choices);
            if (!choice) {
                return false;
            }
            CardOptionTrader opt;
            opt.trash = choice;
            card.playAction(&opt);
            return true;
        }

        case CardId::Cartographer: {
            auto part = getTagged(opt, OptionTag::Discard);
            auto const& choices = anyToList<CardId>(part);
            CardOptionCartographer opt;
            opt.discard = makeDiscardFunc(choices);
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
            auto choice = std::any_cast<int>(getTagged(opt, OptionTag::ChooseOption)) == 1;
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


