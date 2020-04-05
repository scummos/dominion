#pragma once

#include "cost.h"
#include "cardid.h"

// #include <QVarLengthArray>
#include <string>
#include <vector>

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

    enum Hints {
        NoHints = 0x0,
        Chain = 0x1, //< provides +1 or more actions
        Draw = 0x2, //< draws cards
        TerminalDraw = 0x4 | Draw, //< provides +0 actions but draws cards
        Cantrip = Chain | Draw, //< draws at last 1 and gives at least +1 action
        Village = 0x8 | Chain | Draw, //< draws at least 1 and gives at least +2 actions
        VirtualMoney = 0x10, //< gives virtual money
        PlusBuy = 0x20, //< gives +1 or more buy
        Choice = 0x40, //< there are choices when playing this card
        Trasher = 0x80, //< maybe can trash something using this card
    };

    struct BasicInfo {
        CardId id;
        Type types;
        Hints hints;
        Cost cost;
    };

    virtual ~Card() = default;

    // These must be implemented in each derived card.
    virtual BasicInfo basicInfo() const = 0;

    // These by default do nothing or return a zero value, but most cards will override some.
    virtual int victoryPoints() const;

    virtual void playAction(TurnInternal* turn, CardOption* option = nullptr);
    virtual void playTreasure(TurnInternal* turn, CardOption* option = nullptr);

    // Helper functions.
    Type types();
    bool hasType(Type const& type) const;
    char const* name();
    Hints hints();

protected:
    friend class Supply;
    /// Only the supply can create cards. Cards are always moved from the supply
    /// elsewhere.
    Card() = default;
};

// using Cards = QVarLengthArray<Card*, 16>;
using Cards = std::vector<Card*>;

inline Card::Hints operator|(Card::Hints const a, Card::Hints const b) {
    return static_cast<Card::Hints>(int(a) | int(b));
}
