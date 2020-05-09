#pragma once

#include "cardid.h"
#include "condition.h"
#include "genericcardoption.h"

#include <vector>
#include <variant>

using Playorder = std::variant<CardId, std::string>;
using Playorders = std::vector<Playorder>;

class Strategy {
public:
    Strategy() = default;
    Strategy(Playorders const& playorder, GenericOptions const& options);

    void setCondition(Condition::Ptr condition);

    bool conditionFulfilled(Turn* turn) const;
    bool conditionFulfilled(Deck const* deck) const;
    std::optional<GenericCardOption> optionForCard(CardId id, Turn* turn) const;
    std::optional<GenericReactOption> optionForReaction(ReactKind kind, Deck const* deck) const;
    Playorders playorder() const;

private:
    Condition::Ptr m_condition;
    Playorders m_playorder;
    GenericCardOptions m_options;
    GenericReactOptions m_reactions;
};

using StrategyCollection = std::vector<Strategy>;
