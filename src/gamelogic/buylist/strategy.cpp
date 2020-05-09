#include "strategy.h"

Strategy::Strategy(const Playorders& playorder, const GenericOptions& options)
    : m_playorder(playorder)
{
    for (auto const& opt: options) {
        if (std::holds_alternative<GenericCardOption>(opt)) {
            m_options.push_back(std::get<GenericCardOption>(opt));
        }
        else {
            m_reactions.push_back(std::get<GenericReactOption>(opt));
        }
    }
}

void Strategy::setCondition(Condition::Ptr condition)
{
    m_condition = condition;
}

bool Strategy::conditionFulfilled(Turn* turn) const
{
    return !m_condition || m_condition->fulfilled(turn);
}

bool Strategy::conditionFulfilled(Deck const* deck) const
{
    return !m_condition || m_condition->fulfilled(deck);
}

std::optional<GenericCardOption> Strategy::optionForCard(CardId id, Turn* turn) const
{
    auto it = std::find_if(m_options.begin(), m_options.end(), [id, turn](GenericCardOption const& opt) {
        return opt.forCard == id && (!opt.precondition || opt.precondition->fulfilled(turn));
    });
    return it == m_options.end() ? std::nullopt : std::make_optional(*it);
}

std::optional<GenericReactOption> Strategy::optionForReaction(ReactKind kind, Deck const* deck) const
{
    auto it = std::find_if(m_reactions.begin(), m_reactions.end(), [kind, deck](GenericReactOption const& opt) {
        return opt.forKind == kind && (!opt.precondition || opt.precondition->fulfilled(deck));
    });
    return it == m_reactions.end() ? std::nullopt : std::make_optional(*it);
}

Playorders Strategy::playorder() const
{
    return m_playorder;
}

