#include "buylistactor.h"

void BuylistActor::react(EventReactOptions& options)
{
    for (auto& option: options) {
        defaultReact(*option);
    }
}

void BuylistActor::executeTurn(Turn* turn)
{
    info(std::cerr << "*** Turn " << turn->turnCount() << std::endl);

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
                auto playedSomething = true;
                while (playedSomething) {
                    playedSomething = false;
                    auto cards = turn->currentHand().findCards(*cardType);
                    if (!cards.empty()) {
                        playedSomething = genericPlay(turn, cards.back(), strat.optionForCard(*cardType));
                    }
                }
            }
            else {
                auto composite = std::get<std::string>(entry);
                info(std::cerr << "executing composite strategy " << composite << std::endl);
                if (composite == "ChainDraw") {
                    defaultVillageDraw(turn, 0);
                }
                else if (composite == "Default") {
                    defaultVillageDraw(turn, 0);
                    defaultPlayAll(turn);
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

    auto treasureOption = [&usedStrategies](CardId id) -> std::optional<GenericCardOption> {
        for (auto used: usedStrategies) {
            if (auto opt = used->optionForCard(id)) {
                return opt;
            }
        }
        return std::nullopt;
    };

    auto hand = turn->currentHand();
    while (hand.hasCard(Card::Treasure)) {
        for (auto& hcard: hand.cards) {
            if (hcard.card->hasType(Card::Treasure)) {
                genericPlayTreasure(turn, hcard, treasureOption(hcard.card->id()));
            }
        }
        hand = turn->currentHand();
    }

    buylists.buy(turn);
}

