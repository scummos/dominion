#pragma once

#include "supply.h"
#include "deck.h"
#include "actor.h"

#include <vector>
#include <memory>

class Game {
public:
    Game(std::vector<std::string> const& actors);

    Cards createStartingDeck();

    int run();
    bool gameEnded();

    int numPlayers() const;
    int playerIndex(Deck* deck) const;

private:
    Supply m_supply;
    std::vector<Deck> m_players;
    std::vector<std::unique_ptr<Actor>> m_actors;
};
