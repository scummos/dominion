#pragma once

#include "turn.h"
#include "genericcardoption.h"
#include "defaultplay.h"

bool genericPlay(Turn* turn, ActiveCard card, std::optional<GenericCardOption> opt);

void genericPlayTreasure(Turn* turn, ActiveCard card, std::optional<GenericCardOption> opt);
