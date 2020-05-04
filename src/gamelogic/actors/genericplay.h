#pragma once

#include "turn.h"
#include "genericcardoption.h"
#include "defaultplay.h"

bool genericPlay(Turn* turn, ActiveCard card, std::any opt);

void genericPlayTreasure(Turn* turn, ActiveCard card, std::any opt);
