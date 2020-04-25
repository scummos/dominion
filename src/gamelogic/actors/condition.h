#include "turn.h"
#include "type_traits"

class Condition {
public:
    using Ptr = std::unique_ptr<Condition>;

    virtual bool fulfilled(Turn* turn);
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

class AllOf : public Condition {
public:
    AllOf(std::initializer_list<Condition::Ptr> conds) {
        m_conds = conds;
    }

    bool fulfilled(Turn* turn) override {
        return std::all_of(m_conds.begin(), m_conds.end(), [turn](Condition::Ptr p) {
            return p->fulfilled(turn);
        });
    }

private:
    std::vector<Condition::Ptr> m_conds;
};

template<typename T, typename... Args>
Condition::Ptr If(Args... args) {
    static_assert(std::is_base_of_v<Condition, T>, "If must instantiate a Condition");
    return std::make_unique<T>(args...);
}
