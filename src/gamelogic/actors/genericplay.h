#pragma once

#include "turn.h"
#include "genericcardoption.h"
#include "reaction.h"
#include "defaultplay.h"

bool genericPlay(Turn* turn, ActiveCard card, std::function<std::any(CardId card)> optFunc);

inline bool genericPlayStatic(Turn* turn, ActiveCard card, std::any opt) {
    return genericPlay(turn, card, [&opt](CardId) { return opt; });
}

bool genericReact(Deck const* deck, EventReactOption::Ptr reactOption, std::any opt);

void genericPlayTreasure(Turn* turn, ActiveCard card, std::any opt);
