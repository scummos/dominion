#pragma once

class PayResult {
public:
    bool successful() const;
    int goldRemain() const;

    operator bool() const;

private:
    friend class Cost;
    int m_goldRemain;
};

struct Resources {
    int gold;
};

class Cost {
public:
    Cost() = default;
    Cost(int gold);

    bool operator==(Cost const& other) const {
        return m_gold == other.m_gold;
    }
    bool operator!=(Cost const& other) const {
        return !operator==(other);
    }

    PayResult canPay(Resources const& resources) const;
    int gold() const { return m_gold; }
    bool hasAdvancedCost() const { return false; }
    bool valid() const { return m_gold >= 0; }

public: // FIXME
    int m_gold = 0;
};
