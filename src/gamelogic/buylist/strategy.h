#pragma once

#include "cardid.h"
#include "condition.h"
#include "genericcardoption.h"

#include <vector>
#include <variant>

using Playorder = std::variant<CardId, std::string>;

class Strategy {
public:
    Strategy(Playorder const& playorder, GenericCardOptions const& options);

    void setCondition(Condition::Ptr condition);

private:
    Condition::Ptr m_condition;
    Playorder m_playorder;
    GenericCardOptions m_options;
};

using StrategyCollection = std::vector<Strategy>;
