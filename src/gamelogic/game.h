#pragma once

#include "supply.h"
#include "deck.h"
#include "actor.h"

#include <vector>
#include <memory>

class Game {
public:
    Game();

    Deck createStartingDeck();

    void run();
    bool gameEnded();

private:
    Supply m_supply;
    std::vector<Deck> m_players;
    std::vector<std::unique_ptr<Actor>> m_actors;
};
