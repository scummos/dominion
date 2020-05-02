#include "strategy.h"

Strategy::Strategy(const Playorders& playorder, const GenericCardOptions& options)
    : m_playorder(playorder)
    , m_options(options)
{
}

void Strategy::setCondition(Condition::Ptr condition)
{
    m_condition = condition;
}

bool Strategy::conditionFulfilled(Turn* turn) const
{
    return !m_condition || m_condition->fulfilled(turn);
}

std::optional<GenericCardOption> Strategy::optionForCard(CardId id) const
{
    auto it = std::find_if(m_options.begin(), m_options.end(), [id](GenericCardOption const& opt) {
        return opt.forCard == id;
    });
    return it == m_options.end() ? std::nullopt : std::make_optional(*it);
}

Playorders Strategy::playorder() const
{
    return m_playorder;
}

