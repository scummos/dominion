#include "condition.h"
#include "cardid.h"

struct BuyAction {
    // This condition must be fulfilled in addition to
    //  - can actually pay for the card and
    //  - pile of card is not empty.
    Condition::Ptr condition;
    CardId card;
};

struct Buylist {
    Condition::Ptr precondition;
    std::vector<BuyAction> actions;

    CardId select(Turn* turn) {
        for (auto& action: actions) {
            if (turn->leftInSupply(action.card) == 0) {
                continue;
            }
            if (turn->cardCost(action.card).canPay({turn->currentMoney()})) {
                continue;
            }
            if (action.condition && !action.condition->fulfilled(turn)) {
                continue;
            }
            return action.card;
        }
        return CardId::Invalid;
    };
};

struct BuylistCollection {
    std::vector<Buylist> buylists;

    void buy(Turn* turn) {
        for (auto& list: buylists) {
            if (list.precondition && !list.precondition->fulfilled(turn)) {
                continue;
            }

            CardId choice = list.select(turn);
            while (turn->currentBuys() > 0 && choice != CardId::Invalid) {
                turn->buy(choice);
                choice = list.select(turn);
            }
        }
    }
};
