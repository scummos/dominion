#pragma once

#include "cardpile.h"
#include "supply.h"

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
    Deck(std::vector<Card*> startingCards);
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

    /// Gain a card identified by @p id from the supply @p supply, putting it to @p targetArea.
    void gainFromSupply(Supply* supply, CardId const id, Areas targetArea = Areas::DiscardPile);

    /// Trash the given @p card from @p sourceArea, putting it into @p supply's trash.
    void trash(Supply* supply, Card* card, Areas sourceArea);

    /// Convenience getters for the piles.
    auto const& drawPile() const          { return area(Areas::DrawPile); }
    auto const& uncoveredDrawPile() const { return area(Areas::UncoveredDrawPile); }
    auto const& discardPile() const       { return area(Areas::DiscardPile); }
    auto const& hand() const              { return area(Areas::Hand); }

    int countScore() const;
    int totalCards() const;

private:
    auto& drawPile()                { return area(Areas::DrawPile); }
    auto& uncoveredDrawPile()       { return area(Areas::UncoveredDrawPile); }
    auto& discardPile()             { return area(Areas::DiscardPile); }
    auto& hand()                    { return area(Areas::Hand); }
    CardPile& area(Areas area);

private:
    std::vector<CardPile> m_areas;
    Deck(Deck const& other) = delete;
};

