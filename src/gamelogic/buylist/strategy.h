#pragma once

#include "cardid.h"
#include "condition.h"
#include "genericcardoption.h"

#include <vector>
#include <variant>

using Playorder = std::variant<CardId, std::string>;
using Playorders = std::vector<std::variant<CardId, std::string>>;

class Strategy {
public:
    Strategy() = default;
    Strategy(Playorders const& playorder, GenericCardOptions const& options);

    void setCondition(Condition::Ptr condition);

    bool conditionFulfilled(Turn* turn) const;
    std::optional<GenericCardOption> optionForCard(CardId id, Turn* turn) const;
    Playorders playorder() const;

private:
    Condition::Ptr m_condition;
    Playorders m_playorder;
    GenericCardOptions m_options;
};

using StrategyCollection = std::vector<Strategy>;
