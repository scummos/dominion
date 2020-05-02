#include "genericplay.h"

#include "remodel.h"

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

    using CardPair = std::pair<CardId, CardId>;
}

bool genericPlay(Turn* turn, ActiveCard card, std::optional<GenericCardOption> opt)
{
    if (turn->currentActions() <= 0) {
        return false;
    }
    if (!opt || !(card.card->hints() & Card::Choice)) {
        return defaultPlay(turn, card);
    }

    switch (card.card->id()) {
        case CardId::Remodel: {
            auto hand = turn->currentHand();
            hand.ignore(card.card);
            auto const& choices = anyToList<CardPair>(opt->option);
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
        default: break;
    }

    std::cerr << "failed: " << card.card->name() << std::endl;
    throw InvalidPlayError{"Cannot generic-play this action"};
}

void genericPlayTreasure(Turn* turn, ActiveCard card, std::optional<GenericCardOption> opt)
{
    if (!opt || !(card.card->hints() & Card::Choice)) {
        defaultPlayTreasure(turn, card);
        return;
    }

    switch (card.card->id()) {
        case CardId::IllGottenGains: {
            auto choice = std::any_cast<bool>(opt->option);
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


