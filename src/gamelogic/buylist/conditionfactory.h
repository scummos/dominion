#pragma once

#include "condition.h"
#include "defaultcondition.h"

#include <any>

struct InvalidConditionError {
    std::string error;
};

using VariantList = std::vector<std::any>;

Condition::Ptr createCondition(std::string const& condition, VariantList const& args);
