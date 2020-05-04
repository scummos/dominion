#include "event_helpers.h"

#include "event.h"
#include "card.h"

bool eventGainsThisCard(const Event& event, const Card* card)
{
    if (event.kind != EventKind::YouGainACard) {
        return false;
    }
    auto& cardGainEvent = static_cast<YouGainACardEvent const&>(event);
    return cardGainEvent.card == card;
}

Card* cardGainedBySelf(const Event& event)
{
    if (event.kind == EventKind::YouGainACard) {
        return static_cast<YouGainACardEvent const&>(event).card;
    }
    return nullptr;
}
