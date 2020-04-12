#pragma once

#include <vector>
#include <string>

class Deck;
class Game;

// The index in this vectors is the turn number.
using DataSequence = std::vector<int>;

// These data points are sampled once per game and per player.
enum class PerGameLogData {
    WonGame,
    TotalScore,
    TotalCards,
    TotalMoney,
    Num
};

// These data points are sampled at the end of each turn.
enum class PerTurnLogData {
    TurnNumber, // having this makes some things easier, even though it has no extra information
    TurnPeakMoney,
    CardsSeen,
    TotalScore,
    TotalCards,
    TotalMoney,
    Num
};

struct InvalidIndexError {
    std::string error;
};

struct DataPoint {
    double value;
    double error;
};

struct HistogramDimension {
    PerTurnLogData dim;
    int bins;
    double lower, upper;
};

using MultiIndex = std::vector<int>;

class HistogramNd {
public:
    HistogramNd(std::vector<HistogramDimension> const& dims);
    int& value(MultiIndex const& index);
    int valueToRowIndex(PerTurnLogData dim, double value);
    std::vector<int> data() const;

private:
    const std::vector<HistogramDimension> m_dims;
    std::vector<int> m_data;
};

class Logger {
public:
    Logger(int preallocGames);
    static Logger* instance(int preallocGames=1000);

    /// Add per-game data. Caller must ensure this is called exactly once per game, per player, and
    /// per value of @p which.
    void addData(int playerIndex, PerGameLogData which, int value);

    /// Add turn-specific data to the log.
    /// Caller must ensure this is used exactly once per turn, per player, and per value of @p which.
    void addData(int playerIndex, PerTurnLogData which, int value);

    /// Mark the current game as completed, all future calls adding turn data will add it
    /// to a new game.
    void nextGame();

    /// Computes the average per-turn value of @p which for player @p playerIndex.
    std::vector<DataPoint> computeTurnGraph(int playerIndex, PerTurnLogData which);

    HistogramNd computeHistogramNd(int playerIndex, std::vector<HistogramDimension> const& dims);

private:
    struct PlayerData {
        PlayerData();
        DataSequence m_vectorData[(int) PerTurnLogData::Num];
        int m_scalarData[(int) PerGameLogData::Num];
    };
    // The index in this vector is the player number.
    using GamePlayerData = std::vector<PlayerData>;

    // The index in this vector is the game number. Entries are added when calling
    // nextGame().
    std::vector<GamePlayerData> m_games;

private:
    GamePlayerData& currentGame();
    PlayerData& currentPlayerData(int index);
};
