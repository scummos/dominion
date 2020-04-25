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
    Cost(int gold);

    PayResult canPay(Resources const& resources) const;
    int gold() const { return m_gold; }
    bool hasAdvancedCost() const { return false; }

public: // FIXME
    int m_gold;
};
