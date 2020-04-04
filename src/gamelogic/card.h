#pragma once

#include "cost.h"

#include <vector>
#include <string>

struct TurnInternal;

struct CardOption { };

struct InvalidCardUsage {
    std::string err;
};

class Card {
public:
    enum Type {
        Victory = 0x1,
        Treasure = 0x2,
        Action = 0x4,
    };

    virtual ~Card() = default;

    virtual Cost cost() const = 0;
    virtual Type types() const = 0;
    bool hasType(Type const& type) const;

    virtual int victoryPoints() const;

    virtual void playAction(TurnInternal* turn, CardOption* option = nullptr);
    virtual void playTreasure(TurnInternal* turn, CardOption* option = nullptr);

protected:
    friend class Supply;
    /// Only the supply can create cards. Cards are always moved from the supply
    /// elsewhere.
    Card() = default;
};

using Cards = std::vector<Card*>;
