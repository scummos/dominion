#pragma once

#include "cardid.h"
#include "condition.h"
#include <any>

struct GenericCardOption {
    CardId forCard = CardId::Invalid;
    Condition::Ptr precondition;
    std::any option;
};

enum class OptionTag {
    Invalid,
    Discard,
    Keep,
    Trash
};

struct TaggedExpr {
    OptionTag tag = OptionTag::Invalid;
    std::any option;
};

struct GenericReactOption {
    CardId forCard = CardId::Invalid;
    std::any option;
};

using GenericCardOptions = std::vector<GenericCardOption>;
