#pragma once

#include "turn.h"
#include "torturer.h"

void defaultVillageDraw(Turn* turn, int wantActionsRemain = 0);
Cards garbageCards(Cards const& hand);

int usefulness(Card* card);

Card* leastUsefulCardInHand(Hand const& hand, Card* exclude = nullptr);

int plainTreasureInHand(Cards const& hand);

enum class CardSortOrder {
    LeastExpensive,
    MostExpensive
};

Card* worstTreasure(Hand& hand, CardSortOrder order = CardSortOrder::LeastExpensive);

CardId upgradeTreasure(CardId id);

void defaultReact(Deck const* deck, EventReactOption::Ptr option);
