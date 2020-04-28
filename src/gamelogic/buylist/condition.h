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

class Negate : public Condition {
public:
    Negate(Condition::Ptr cond) : m_cond(cond) {}
    static auto create(Condition::Ptr cond) { return std::make_shared<Negate>(cond); }

    bool fulfilled(Turn* turn) override {
        return m_cond && !m_cond->fulfilled(turn);
    }

private:
    Condition::Ptr m_cond;
};

class AllOf : public Condition {
public:
    template<typename... Args>
    AllOf(Args... conds) {
        m_conds = {conds...};
    }

    template<typename... Args>
    static auto create(Args... conds) { return std::make_shared<AllOf>(conds...); }

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
    template<typename... Args>
    static auto create(Args... conds) { return std::make_shared<AnyOf>(conds...); }

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

template<typename T, typename... Args>
Condition::Ptr IfNot(Args... args) {
    static_assert(std::is_base_of_v<Condition, T>, "If must instantiate a Condition");
    return std::make_shared<Negate>(std::make_shared<T>(args...));
}
