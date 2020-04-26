#pragma once

#include "condition.h"

// Some convenience conditions.

class HasMoney : public Condition {
public:
    HasMoney(int money) : m_money(money) {}

    bool fulfilled(Turn* turn) override {
        return turn->currentMoney() >= m_money;
    }

private:
    int m_money;
};

class TurnCountLessThan : public Condition {
public:
    TurnCountLessThan(int count) : m_count(count) {}

    bool fulfilled(Turn* turn) override {
        return turn->turnCount() < m_count;
    }

private:
    int m_count;
};

class CardCountLessThan : public Condition {
public:
    CardCountLessThan(CardId id, int count) : m_id(id), m_count(count) {}

    bool fulfilled(Turn* turn) override {
        return turn->totalCards(m_id) < m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class CardCountGreaterThan : public Condition {
public:
    CardCountGreaterThan(CardId id, int count) : m_id(id), m_count(count) {}

    bool fulfilled(Turn* turn) override {
        return turn->totalCards(m_id) > m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class Buy2ndDelayedAfter : public Condition {
public:
    Buy2ndDelayedAfter(CardId id, CardId delayTo) : m_id(id), m_delayTo(delayTo) { }

    bool fulfilled(Turn* turn) override {
        auto count = turn->totalCards(m_id);
        return count == 0 || (count == 1 && turn->totalCards(m_delayTo) >= 1);
    }

private:
    CardId m_id;
    CardId m_delayTo;
};

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
