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

char const* Card::name()
{
    return cardName(basicInfo().id);
}

Card::Hints Card::hints()
{
    return basicInfo().hints;
}

Card::Type Card::types()
{
    return basicInfo().types;
}

CardTraits Card::traits() const
{
    return {};
}
