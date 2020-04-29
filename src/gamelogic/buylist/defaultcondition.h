#pragma once

#include "condition.h"

// Some convenience conditions.

class HasMoney : public Condition {
public:
    HasMoney(int money) : m_money(money) {}
    static auto create(int money) { return std::make_shared<HasMoney>(money); }

    bool fulfilled(Turn* turn) override {
        return turn->currentMoney() >= m_money;
    }

private:
    int m_money;
};

class Has : public Condition {
public:
    Has(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<Has>(id, count); }

    bool fulfilled(Turn* turn) override {
        return turn->totalCards(m_id) >= m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class HasExact : public Condition {
public:
    HasExact(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<HasExact>(id, count); }

    bool fulfilled(Turn* turn) override {
        return turn->totalCards(m_id) == m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class SupplyHasLess : public Condition {
public:
    SupplyHasLess(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<SupplyHasLess>(id, count); }

    bool fulfilled(Turn* turn) override {
        return turn->leftInSupply(m_id) < m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class SupplyEmptyPilesLess : public Condition {
public:
    SupplyEmptyPilesLess(int count) : m_count(count) {}
    static auto create(int count) { return std::make_shared<SupplyEmptyPilesLess>(count); }

    bool fulfilled(Turn* turn) override {
        return turn->emptySupplyPiles() < m_count;
    }

private:
    int m_count;
};

class TurnCountLessThan : public Condition {
public:
    TurnCountLessThan(int count) : m_count(count) {}
    static auto create(int count) { return std::make_shared<TurnCountLessThan>(count); }

    bool fulfilled(Turn* turn) override {
        return turn->turnCount() < m_count;
    }

private:
    int m_count;
};

class CardCountLessThan : public Condition {
public:
    CardCountLessThan(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<CardCountLessThan>(id, count); }

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
    static auto create(CardId id, int count) { return std::make_shared<CardCountGreaterThan>(id, count); }

    bool fulfilled(Turn* turn) override {
        return turn->totalCards(m_id) > m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class LateGame : public Condition {
public:
    LateGame() = default;
    static auto create() { return new LateGame; }

    bool fulfilled(Turn* turn) override {
        return turn->leftInSupply(CardId::Province) <= 3;
    };
};

class VeryLateGame : public Condition {
public:
    VeryLateGame() = default;
    static auto create() { return std::make_shared<VeryLateGame>; }

    bool fulfilled(Turn* turn) override {
        return turn->leftInSupply(CardId::Province) <= 2;
    };
};
