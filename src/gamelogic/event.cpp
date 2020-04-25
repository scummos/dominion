#include "event.h"
#include "reaction.h"

Event::Event(EventKind kind)
    : kind(kind)
{
}

AttackEvent::AttackEvent(EventReactOption::Ptr attack)
    : Event(EventKind::Attack)
    , attack(attack)
{
}
