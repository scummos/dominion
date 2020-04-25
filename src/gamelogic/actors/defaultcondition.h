#pragma once

#include "condition.h"

// Some convenience conditions.

class LateGame : public Condition {
public:
    LateGame() = default;

    bool fulfilled(Turn* turn) override {
        return turn->leftInSupply(CardId::Province) <= 3;
    };
};

class VeryLateGame : public Condition {
public:
    VeryLateGame() = default;

    bool fulfilled(Turn* turn) override {
        return turn->leftInSupply(CardId::Province) <= 2;
    };
};
