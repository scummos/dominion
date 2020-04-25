#include "game.h"

#include "cards.h"
#include "logger.h"
#include "actor_factory.h"

#include <iostream>

Game::Game(std::vector<std::string> const& actors)
    : m_supply(actors.size())
{
    int i = 0;
    for (auto const& actorName: actors) {
        auto& player = m_players.emplace_back(createStartingDeck(), &m_supply, i);
        m_actors.emplace_back(createActor(actorName, &m_supply, &player));
        i++;
    }

    // TODO: order these such that the first in the list is the next player
    for (int i = 0; i < m_players.size(); i++) {
        std::vector<Deck*> enemies;
        for (int j = 0; j < m_players.size(); j++) {
            if (i == j)
                continue;
            enemies.push_back(&m_players[j]);
        }
        m_players[i].setEnemies(enemies);
    }
}

int Game::playerIndex(Deck* deck) const
{
    for (int i = 0; i < m_players.size(); i++) {
        if (deck == &m_players.at(i)) {
            return i;
        }
    }
    return -1;
}

int Game::numPlayers() const
{
    return m_players.size();
}

Cards Game::createStartingDeck()
{
    std::vector<Card*> cards;
    cards.reserve(10);
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

int Game::run()
{
    Logger::instance()->nextGame();

    int currentPlayer = 0;
    for (auto& player: m_players) {
        Turn turn(&m_supply, &player);
        turn.doFinalDraw();
    }

    int turncount = 0;
    while (!gameEnded()) {
        auto* player = &m_players.at(currentPlayer);
        auto& actor = m_actors.at(currentPlayer);

        Turn turn(&m_supply, player);
        actor->executeTurn(&turn);
        turn.endTurn();

        currentPlayer = (currentPlayer + 1) % m_players.size();
        turncount++;
    }

    auto maxScore = -10000, winner = -1, minTurns = 0;
    for (int i = 0; i < m_players.size(); i++) {
        auto score = m_players.at(i).countScore();
        auto turns = m_players.at(i).turnCount();
        if (score > maxScore || (score == maxScore && minTurns > turns)) {
            winner = i;
            maxScore = score;
            minTurns = turns;
        }
        else if (score == maxScore) {
            winner = -1;
        }

        Logger::instance()->addData(i, PerGameLogData::TotalScore, score);
    }

    for (int i = 0; i < m_players.size(); i++) {
        Logger::instance()->addData(i, PerGameLogData::WonGame, i == winner);
    }

    return winner;
}


