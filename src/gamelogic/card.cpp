#include "card.h"

bool Card::hasType(const Card::Type& type) const
{
    return types() & type;
}

void Card::playAction(Turn* /*turn*/, CardOption* /*option*/)
{
    throw InvalidCardUsage{"This card cannot be played as an action."};
}

void Card::playTreasure(Turn* /*turn*/, CardOption* /*option*/)
{
    throw InvalidCardUsage{"This card cannot be played as a treasure."};
}

int Card::victoryPoints() const
{
    return 0;
}
