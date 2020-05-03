#include "game.h"

#include "cards.h"
#include "logger.h"
#include "actor_factory.h"

#include <iostream>

Game::Game(std::vector<std::string> const& actors, std::vector<std::any> const& args)
    : m_supply(actors.size())
{
    int i = 0;
    for (auto const& actorName: actors) {
        auto& player = m_players.emplace_back(createStartingDeck(), &m_supply, i);
        m_actors.emplace_back(createActor(actorName, &m_supply, &player, args[i]));
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

    m_logData.resize(m_players.size());
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
    int currentPlayer = 0;
    for (auto& player: m_players) {
        Turn turn(&m_supply, &player, m_logData[currentPlayer]);
        turn.doFinalDraw();
        currentPlayer++;
    }

    for (int i = 0; i < m_players.size(); i++) {
        auto& actor = m_actors[i];
        Deck::ReactCallback cb = [&actor](EventReactOptions& options) { actor->react(options); };
        m_players[i].setReactCallback(cb);
    }

    int turncount = 0;
    currentPlayer = m_firstPlayer;
    while (!gameEnded() && turncount < 63) {
        auto* player = &m_players.at(currentPlayer);
        auto& actor = m_actors.at(currentPlayer);

        Turn turn(&m_supply, player, m_logData[currentPlayer]);
        actor->executeTurn(&turn);
        turn.endTurn();

        currentPlayer = (currentPlayer + 1) % m_players.size();
        turncount++;
    }

    struct Score {
        int player;
        int vp;
        int turns;
    };
    std::vector<Score> scores;
    int i = 0;
    for (auto const& player: m_players) {
        scores.emplace_back(Score{
            i, player.countScore(), player.turnCount()
        });
        m_logData[i].addData(PerGameLogData::TotalScore, scores.back().vp);
        i++;
    }

    std::sort(scores.begin(), scores.end(), [](Score const& s1, Score const& s2) {
        return s1.vp == s2.vp ? s1.turns > s2.turns : s1.vp < s2.vp;
    });

    auto first = scores.at(scores.size() - 1);
    auto second = scores.at(scores.size() - 2);
    auto winner = first.player;
    if (first.vp == second.vp && first.turns == second.turns) {
        winner = -1;
    }

    for (int i = 0; i < m_players.size(); i++) {
        m_logData[i].addData(PerGameLogData::WonGame, i == winner);
    }

    return winner;
}

void Game::setFirstPlayer(int index) {
    m_firstPlayer = index;
}

Logger::GameData Game::logData() const
{
    return m_logData;
}
