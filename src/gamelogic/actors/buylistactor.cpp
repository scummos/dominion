#include "buylistactor.h"

namespace {
    std::any option(std::optional<GenericCardOption> const& cardOpt) {
        if (!cardOpt.has_value()) {
            return std::any();
        }
        return cardOpt->option;
    }
}

void BuylistActor::react(EventReactOptions& options)
{
    for (auto& reactOption: options) {
        bool handled = false;
        for (auto const& strat: strats) {
            if (!strat.conditionFulfilled(m_deck)) {
                continue;
            }
            auto opt = strat.optionForReaction(reactOption->kind(), m_deck);
            if (!opt.has_value()) {
                continue;
            }
            handled = genericReact(m_deck, reactOption, opt->option);
            if (handled) {
                break;
            }
        }
        if (!handled) {
            defaultReact(m_deck, reactOption);
        }
    }
}

void BuylistActor::executeTurn(Turn* turn)
{
    info(std::cerr << "*** Turn " << turn->turnCount() << std::endl);

    auto playAllByType = [turn](Strategy const& strat, auto cardType) {
        auto playedSomething = true;
        while (playedSomething) {
            playedSomething = false;
            auto cards = turn->currentHand().findCards(cardType);
            if (!cards.empty()) {
                auto const& card = cards.back();
                // We have to pass this as a functor, since genericPlay might need to call it
                // for other cards than the passed one (think Throne Room).
                auto optionFunc = [turn, &strat](CardId id) {
                    return option(strat.optionForCard(id, turn));
                };
                playedSomething = genericPlay(turn, card, optionFunc);
            }
        }
    };

    // Strategies are processed top to bottom, just once.
    std::vector<Strategy const*> usedStrategies;
    for (auto const& strat: strats) {
        if (!strat.conditionFulfilled(turn)) {
            continue;
        }
        usedStrategies.push_back(&strat);
        for (auto const& entry: strat.playorder()) {
            if (auto cardType = std::get_if<CardId>(&entry)) {
                info(std::cerr << "executing playorder entry " << cardName(*cardType) << std::endl);
                playAllByType(strat, *cardType);
            }
            else {
                auto composite = std::get<std::string>(entry);
                info(std::cerr << "executing composite strategy " << composite << std::endl);
                if (composite == "ChainDraw") {
                    defaultVillageDraw(turn, 0);
                }
                else if (composite == "Default") {
                    defaultVillageDraw(turn, 0);
                    playAllByType(strat, Card::Action);
                }
                else {
                    throw InvalidPlayError{"Invalid composite action"};
                }
            }
            if (turn->currentActions() == 0) {
                // nothing left to do here
                break;
            }
        }
        if (turn->currentActions() == 0) {
            // nothing left to do here
            break;
        }
    }

    auto treasureOption = [&usedStrategies, turn](CardId id) -> std::optional<GenericCardOption> {
        for (auto used: usedStrategies) {
            if (auto opt = used->optionForCard(id, turn)) {
                return opt;
            }
        }
        return std::nullopt;
    };

    while (turn->currentHand().hasCard(Card::Treasure)) {
        for (int i = 0; i < turn->currentHandSize(); i++) {
            auto& hcard = turn->currentHand().cards[i];
            if (hcard->hasType(Card::Treasure)) {
                auto active = turn->currentHand().activeCard(hcard);
                genericPlayTreasure(turn, active, option(treasureOption(hcard->id())));
            }
        }
    }

    buylists.buy(turn);
}

