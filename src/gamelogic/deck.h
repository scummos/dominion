#pragma once

#include "cardpile.h"
#include "supply.h"
#include "event.h"
#include "reaction.h"

#include <functional>

enum class Areas {
    DrawPile,
    UncoveredDrawPile,
    DiscardPile,
    Hand,
    InPlay,
    NumAreas
};

// This class simultaneously represents a player.
// The deck has ownership of all contained cards.
class Deck {
public:
    using ReactCallback = std::function<void(EventReactOptions&)>;

    Deck(std::vector<Card*> startingCards, Supply* supply, int playerIndex);
    Deck(Deck&& other);
    ~Deck();

    /// Get a pointer to the given area, e.g. the discard pile:
    /// \code auto discardPile = deck->area(Areas::DiscardPile) \endcode
    CardPile const& area(Areas area) const;

    /// Move the indicated card from area @p from to area @p to.
    void moveCard(Card* card, Areas from, Areas to);

    /// Move the card with index @p index in @p from to @p to.
    void moveCard(int index, Areas from, Areas to);

    /// Move all card in @p from to @p to.
    void moveAllCards(Areas from, Areas to);

    /// Move @p n cards from the draw pile to the "uncovered draw pile" temp area. Returns the
    /// number of cards uncovered.
    int uncoverCards(int n);

    /// Move all cards from the "uncovered draw pile" temp area back on the draw pile,
    /// in order.
    void putUncoveredBack();

    /// Draw up to @p n cards and put them into the hand. Returns the actual number of cards drawn.
    int drawCards(int n);

    /// Gain a card identified by @p id from the supply, putting it to @p targetArea.
    /// Returns true if the card was gained, false if the supply pile was empty.
    bool gainFromSupply(CardId const id, Areas targetArea = Areas::DiscardPile);

    /// Trash the given @p card from @p sourceArea, putting it into the supply's trash.
    void trash(Card* card, Areas sourceArea);

    /// Discard @p card from hand.
    void discardFromHand(Card* card);

    /// Mark that we completed a turn.
    void countTurn();

    /// Return the amount of turns we played so far.
    int turnCount() const;

    /// Handle an attack on this deck.
    void attacked(AttackEvent& event);

    /// Handle some other event occuring (e.g. enemy gains a card, ...)
    void eventOccured(Event& event);

    /// Run @p func for each card in the deck, no matter where it is. This can
    /// for example be used to count score.
    void forEachCard(std::function<void(Card const*)> func) const;

    void setReactCallback(ReactCallback cb);

    EventReactOptions queryCardsForReactions(Event& event);

    /// Get enemies in the game.
    std::vector<Deck*> enemies() const;
    /// Set enemies in the game.
    void setEnemies(std::vector<Deck*> enemies);

    /// Convenience getters for the piles.
    auto const& drawPile() const          { return area(Areas::DrawPile); }
    auto const& uncoveredDrawPile() const { return area(Areas::UncoveredDrawPile); }
    auto const& discardPile() const       { return area(Areas::DiscardPile); }
    auto const& hand() const              { return area(Areas::Hand); }
    auto const& inPlay() const            { return area(Areas::InPlay); }

    int countScore() const;
    int totalCards() const;
    int totalCards(CardId id) const;
    bool hasCardInHand(Card const* card) const;
    int totalMoney() const;
    int playerIndex() const;

private:
    auto& drawPile()                { return area(Areas::DrawPile); }
    auto& uncoveredDrawPile()       { return area(Areas::UncoveredDrawPile); }
    auto& discardPile()             { return area(Areas::DiscardPile); }
    auto& hand()                    { return area(Areas::Hand); }
    auto& inPlay()                  { return area(Areas::InPlay); }
    CardPile& area(Areas area);

private:
    std::vector<CardPile> m_areas;
    std::vector<Deck*> m_enemies;
    Supply* m_supply;
    int m_turnCount = 0;
    int m_playerIndex = -1; // used for logging
    ReactCallback m_react;
    Deck(Deck const& other) = delete;
};

