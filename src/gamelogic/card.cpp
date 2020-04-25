#include "card.h"

bool Card::hasType(const Card::Type& type) const
{
    return basicInfo().types & type;
}

void Card::playAction(TurnInternal* /*turn*/, CardOption* /*option*/)
{
    throw InvalidCardUsage{"This card cannot be played as an action."};
}

void Card::playTreasure(TurnInternal* /*turn*/, CardOption* /*option*/)
{
    throw InvalidCardUsage{"This card cannot be played as a treasure."};
}

int Card::victoryPoints() const
{
    return 0;
}

char const* Card::name() const
{
    return cardName(basicInfo().id);
}

Card::Hints Card::hints() const
{
    return basicInfo().hints;
}

Card::Type Card::types() const
{
    return basicInfo().types;
}

Cost Card::cost() const
{
    return basicInfo().cost;
}

CardTraits Card::traits() const
{
    return {};
}

std::shared_ptr<EventReactOption> Card::reactToEvent(Event& event) const
{
    return {};
}

