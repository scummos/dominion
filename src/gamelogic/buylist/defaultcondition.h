#pragma once

#include "condition.h"
#include "actor_helpers.h"

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

class HasMoneyInHand : public Condition {
public:
    HasMoneyInHand(int money) : m_money(money) {}
    static auto create(int money) { return std::make_shared<HasMoneyInHand>(money); }

    bool fulfilled(Deck const* deck) override {
        return plainTreasureInHand(deck->constHand().cards()) >= m_money;
    }

private:
    int m_money;
};

class HasInHand : public Condition {
public:
    HasInHand(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<HasInHand>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->constHand().count(m_id) >= m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class HasInPlay : public Condition {
public:
    HasInPlay(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<HasInPlay>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->constInPlay().count(m_id) >= m_count;
    }

private:
    CardId m_id;
    int m_count;
};

template<int N>
class HasNInHand : public Condition {
public:
    template<typename... Types>
    HasNInHand(Types... types) : m_ids({types...}) { }

    template<typename... Types>
    static auto create(Types... types) { return std::make_shared<HasNInHand<N>>(types...); }

    bool fulfilled(Deck const* deck) override {
        auto h = deck->constHand();
        int count = 0;
        for (auto const id: m_ids) {
            count += h.count(id);
        }
        return count >= N;
    }

private:
    std::vector<CardId> m_ids;
};

class Has : public Condition {
public:
    Has(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<Has>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->totalCards(m_id) >= m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class HasExact : public Condition {
public:
    HasExact(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<HasExact>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->totalCards(m_id) == m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class SupplyHasLess : public Condition {
public:
    SupplyHasLess(CardId id, int count) : m_id(id), m_count(count) { }
    static auto create(CardId id, int count) { return std::make_shared<SupplyHasLess>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->leftInSupply(m_id) < m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class SupplyEmptyPilesGreater : public Condition {
public:
    SupplyEmptyPilesGreater(int count) : m_count(count) {}
    static auto create(int count) { return std::make_shared<SupplyEmptyPilesGreater>(count); }

    bool fulfilled(Deck const* deck) override {
        return deck->emptySupplyPiles() > m_count;
    }

private:
    int m_count;
};

class TurnCountLessThan : public Condition {
public:
    TurnCountLessThan(int count) : m_count(count) {}
    static auto create(int count) { return std::make_shared<TurnCountLessThan>(count); }

    bool fulfilled(Deck const* deck) override {
        return deck->turnCount() < m_count;
    }

private:
    int m_count;
};

class CardCountLessThan : public Condition {
public:
    CardCountLessThan(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<CardCountLessThan>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->totalCards(m_id) < m_count;
    }

private:
    CardId m_id;
    int m_count;
};

class CardCountGreaterThan : public Condition {
public:
    CardCountGreaterThan(CardId id, int count) : m_id(id), m_count(count) {}
    static auto create(CardId id, int count) { return std::make_shared<CardCountGreaterThan>(id, count); }

    bool fulfilled(Deck const* deck) override {
        return deck->totalCards(m_id) > m_count;
    }

private:
    CardId m_id;
    int m_count;
};

