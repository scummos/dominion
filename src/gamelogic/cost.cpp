#include "cost.h"

#include <iostream>

int PayResult::goldRemain() const
{
    return m_goldRemain;
}

bool PayResult::successful() const
{
    return m_goldRemain >= 0;
}

PayResult::operator bool() const
{
    return successful();
}

PayResult Cost::canPay(Resources const& resources) const
{
    PayResult ret;
    ret.m_goldRemain = resources.gold - m_gold;
    return ret;
}

Cost::Cost(int gold)
    : m_gold(gold)
{
}
