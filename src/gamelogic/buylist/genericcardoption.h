#pragma once

#include "cardid.h"
#include "condition.h"
#include <any>
#include <variant>

struct GenericCardOption {
    CardId forCard = CardId::Invalid;
    Condition::Ptr precondition;
    std::any option;
};

struct GenericReactOption {
    ReactKind forKind;
    Condition::Ptr precondition;
    std::any option;
};

enum class OptionTag {
    Invalid,
    ChooseOption,
    Discard,
    Keep,
    Trash
};

struct TaggedExpr {
    OptionTag tag = OptionTag::Invalid;
    std::any option;
};

using GenericCardOptions = std::vector<GenericCardOption>;
using GenericReactOptions = std::vector<GenericReactOption>;

using GenericOption = std::variant<GenericCardOption, GenericReactOption>;
using GenericOptions = std::vector<GenericOption>;
