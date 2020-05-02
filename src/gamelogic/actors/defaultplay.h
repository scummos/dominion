#pragma once

#include "turn.h"
#include "mine.h"
#include "mandarin.h"
#include "rebuild.h"
#include "oasis.h"
#include "chapel.h"
#include "illgottengains.h"
#include "actor_helpers.h"

#define info(x)

bool defaultPlay(Turn* turn, ActiveCard& card);

void defaultPlayAll(Turn* turn);

bool defaultPlayTreasure(Turn* /*turn*/, ActiveCard& card);
