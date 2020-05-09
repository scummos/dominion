#pragma once

#include "supply.h"
#include "deck.h"
#include "actor.h"
#include "logger.h"

#include <vector>
#include <memory>
#include <any>
#include <functional>

class Game {
public:
    using LogFunction = std::function<void(int, std::string)>;

    Game(std::vector<std::string> const& actors, std::vector<std::any> const& args);

    Cards createStartingDeck();

    int run();
    bool gameEnded();

    int numPlayers() const;
    int playerIndex(Deck* deck) const;

    Logger::GameData logData() const;
    void setLogFunction(LogFunction func);
    void logStartingCards(int index, int turncount, Deck const* player);

    void setFirstPlayer(int index);

private:
    Supply m_supply;
    std::vector<Deck> m_players;
    std::vector<std::unique_ptr<Actor>> m_actors;
    Logger::GameData m_logData;
    int m_firstPlayer = 0;

    LogFunction m_logFunc;
};
