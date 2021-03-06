#include "defaultplay.h"
#include "genericplay.h"

#define info(x)

bool defaultPlay(Turn* turn, ActiveCard& card)
{
    if ((card.requiresActionToPlay && turn->currentActions() <= 0) || !card.card->hasType(Card::Action)) {
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

        case CardId::Embassy:
            return genericPlayStatic(turn, card, std::vector<std::any>{CardId::Curse, CardId::Estate, CardId::Duchy, CardId::Province, CardId::Copper});

        case CardId::Trader:
            return genericPlayStatic(turn, card, std::vector<std::any>{CardId::Estate});

        case CardId::Steward:
            return genericPlayStatic(turn, card, std::vector<TaggedExpr>{TaggedExpr{OptionTag::ChooseOption, 1}});

        default: break;
    }
    info(std::cerr << "Info: cannot default-play card " << card.card->name() << std::endl);
    return false;
}

void defaultPlayAll(Turn* turn)
{
    auto didPlaySometing = true;
    while (didPlaySometing) {
        didPlaySometing = false;
        info(std::cerr << " --- " << std::endl);
        for (auto* card: turn->currentHand().cards) {
            info(std::cerr << "  Card " << card->name() << std::endl);
            auto active = turn->currentHand().activeCard(card);
            didPlaySometing = defaultPlay(turn, active);
            if (didPlaySometing) {
                // Might be that our hand changed
                info(std::cerr << "* Card " << card->name() << " played" << std::endl);
                break;
            }
        }
    }
}

bool defaultPlayTreasure(Turn*, ActiveCard& card)
{
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
