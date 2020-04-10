#include "game.h"

#include "cards.h"
#include "bigmoneyactor.h"
#include "engineactor.h"
#include "shepherdactor.h"

#include <iostream>

Game::Game()
{
    m_players.emplace_back(createStartingDeck());
}

Cards Game::createStartingDeck()
{
    std::vector<Card*> cards;
    for (int i = 0; i < 7; i++) {
        cards.push_back(m_supply.pile(CardId::Copper).draw());
    }
    for (int i = 0; i < 3; i++) {
        cards.push_back(m_supply.pile(CardId::Estate).draw());
    }
    return cards;
}

bool Game::gameEnded()
{
    return m_supply.pile(CardId::Province).empty() || m_supply.countEmptyPiles() >= 3;
}

void Game::run()
{
    m_actors.emplace_back(std::make_unique<BigMoneyActor>(&m_supply, &m_players.at(0)));

    int currentPlayer = 0;
    for (auto& player: m_players) {
        Turn turn(&m_supply, &player);
        turn.doFinalDraw();
    }

    int turncount = 0;
    while (!gameEnded()) {
        auto* player = &m_players.at(currentPlayer);
        Turn turn(&m_supply, player);
        m_actors.at(currentPlayer)->executeTurn(&turn);
        turn.endTurn();
        turn.doFinalDraw();
        currentPlayer = (currentPlayer + 1) % m_players.size();
        turncount++;
    }

    for (auto& player: m_players) {
        std::cout << turncount << "\t" << player.countScore() << "\n";
    }
}


