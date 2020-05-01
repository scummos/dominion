#pragma once

#include "cardid.h"
#include "condition.h"
#include <any>

struct GenericCardOption {
    CardId forCard = CardId::Invalid;
    Condition::Ptr precondition;
    std::any option;
};

using GenericCardOptions = std::vector<GenericCardOption>;
