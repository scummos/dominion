#pragma once

#include "cost.h"
#include "cardid.h"
#include "error.h"

// #include <QVarLengthArray>
#include <string>
#include <vector>
#include <memory>

struct TurnInternal;
struct Event;
struct EventReactOption;
class Deck;

struct CardOption { };

struct InvalidCardUsage {
    std::string err;
};

struct CardTraits {
    int treasureValue = 0;
};

class Card {
public:
    enum Type {
        NoType = 0x0,
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
        Attack = 0x100, //< an attack card
    };

    struct BasicInfo {
        CardId id = CardId::Invalid;
        Type types = NoType;
        Hints hints = NoHints;
        Cost cost = {};
    };

    virtual ~Card() = default;

    // This must be implemented in each derived card.
    BasicInfo const& basicInfo() const;

    // This can be overriden by cards to ease their generic use.
    virtual CardTraits traits() const;

    // These by default do nothing or return a zero value, but most cards will override some.
    /// How many victory points this card is worth.
    virtual int victoryPoints() const;

    /// Return a structure describing possible reactions if this card reacts to the given event.
    virtual std::shared_ptr<EventReactOption> reactToEvent(Event& event, Deck* playerDeck) const;

    virtual void playAction(TurnInternal* turn, CardOption* option = nullptr);
    virtual void playTreasure(TurnInternal* turn, CardOption* option = nullptr);

    // Helper functions.
    Type types() const;
    bool hasType(Type const& type) const;
    char const* name() const;
    Hints hints() const;
    Cost cost() const;
    CardId id() const;

protected:
    friend class Supply;
    /// Only the supply can create cards. Cards are always moved from the supply
    /// elsewhere.
    Card() = default;

    BasicInfo m_info;
};

// using Cards = QVarLengthArray<Card*, 16>;
using Cards = std::vector<Card*>;

inline Card::Hints operator|(Card::Hints const a, Card::Hints const b) {
    return static_cast<Card::Hints>(int(a) | int(b));
}
