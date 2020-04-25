#pragma once

#include "turn.h"
#include <type_traits>

#include <iostream>

class Condition {
public:
    using Ptr = std::shared_ptr<Condition>;
    static Ptr None() { return Ptr(); };

    virtual ~Condition() = default;

    virtual bool fulfilled(Turn* turn) { return true; };
};

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

class AllOf : public Condition {
public:
    template<typename... Args>
    AllOf(Args... conds) {
        m_conds = {conds...};
    }

    bool fulfilled(Turn* turn) override {
        return std::all_of(m_conds.begin(), m_conds.end(), [turn](Condition::Ptr p) {
            return p->fulfilled(turn);
        });
    }

private:
    std::vector<Condition::Ptr> m_conds;
};

class AnyOf : public Condition {
public:
    template<typename... Args>
    AnyOf(Args... conds) {
        m_conds = {conds...};
    }

    bool fulfilled(Turn* turn) override {
        return std::any_of(m_conds.begin(), m_conds.end(), [turn](Condition::Ptr p) {
            return p->fulfilled(turn);
        });
    }

private:
    std::vector<Condition::Ptr> m_conds;
};

template<typename T, typename... Args>
Condition::Ptr If(Args... args) {
    static_assert(std::is_base_of_v<Condition, T>, "If must instantiate a Condition");
    return std::make_shared<T>(args...);
}
