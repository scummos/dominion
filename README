A program which plays Dominion. Start the GUI (dominion_qt) and play away!

Contribute
==========
Please contribute! I implemented some cards, but far from all of them. I hope that the code
is now in a state where adding more cards is not super complicated; at least for the common things happening,
there is very often a similar card already.
Implementing a card is usually between 20 and 80 lines of code for the card, plus another 5-10 for integrating
it into the language explained below (see src/gamelogic/actors/genericplay.cpp).

See bottom of this file for a super quick introduction.

Strategy file syntax
====================

The syntax of the strategy files looks something like this:

```
buylists
	list if AnyOf(SupplyHasLess(Province, 2), SupplyEmptyPilesGreater(1))
		Province, Duchy, Estate
	end
	list if SupplyHasLess(Province, 4)
		Province, Duchy
	end
	list
		Province if Has(IllGottenGains, 5),
		Smithy [1],
		IllGottenGains,
		Silver [2]
	end
end

strategies
	strategy
		playorder
			Smithy,
			:Default
		end
		options for IllGottenGains
			choose option
		end
	end
end
```

Pretty much all sections are required; the minimum file is similar to this:
```
buylists
	list
		Estate
	end
end

strategies
	strategy
		playorder
			:Default
		end
	end
end
```

Here's a quick primer on how this all works. I'm not actually sure how complex the language is for somebody who has
not designed it, so it's entirely possible you don't need to read any of this and can just look at the numerous included
examples.

First of all, newlines and extra spaces are always ignored.

The buylists section
--------------------
 * The `buylists` section describes what cards are bought in the Buy phase of the turn. It contains multiple lists.
 * Each list has a condition (which is optional), and contains a sequence of card names, separated by commas ,
 * The lists are processed in order, top to bottom, until a list with a fulfilled condition and at least one applicable
    entry is found.
 * The first applicable card, top to bottom, is selected from the list.
 * A card is applicable if its condition is fulfilled and you have enough money to buy the card.
 * If buys remain, processing then restarts from the top.
 * Conditions can be specified on a whole list (the whole list is skipped if the condition is not fulfilled),
   and on individual entries. The idea is to use different lists for different phases of the game, and conditions
   on the individual cards in a list for control how many to buy and in what order.
 * There is a short form for "if not Has(Foo, N)" which is [N]. So, Smithy [2] will buy a Smithy unless your
   deck already contains 2 or more.
 * A card can occur more than once in a list.

The strategies section
----------------------
 * The strategies section describes in what order cards are played, and which choices are made if the cards offer them.
 * It consists of at least one `strategy` section; these sections can be made conditional, like for buylists: use
   `strategy if ...`.
 * For each action to perform, Strategies are processed top to bottom, until one with a matching condition is found.
   That strategy is used for that action. If another action is performed, search starts from the top again.
 * Strategies have three sub-entries: `playorder`, which configures in what order cards are played; `options for CardName`,
   which configures how to play CardName; and `reactions for EventName`, which configures how to handle certain events.
 * The `playorder` section must come first and there must be exactly one for each strategy.

### The playorder section
A playorder section simply lists which card(s) to play, in order. As soon as one card is played, processing stops and
completely re-starts from the top of the strategies section.
There are two special entries available, starting with a colon:
 * `:ChainDraw` plays option-less cards giving plus-actions until no more are available, then plays at most one option-less
   card which draws more cards, and loops until it cannot do anything any more.
 * `:Default` first executes `:ChainDraw`, then plays the most expensive card from your hand it knows how to play.

### The options for / reactions for section
`options for` is the most powerful tool of this application. It allows to configure how cards behave, and even modify this
behaviour depending on other circumstances. Its body differs depending on the card, but always has one of the following forms:
 * Card list: `Copper, Estate, Curse` (used for e.g. Chapel)
 * Card association list: `Copper -> Estate, Estate -> Silver, Gold -> Province` (used for e.g. Remodel)
 * Yes/No decision: `choose option` or `ignore option` (e.g. Ill-Gotten Gains)
 * Choice: `choose option 1`, options are numbered starting from 1 in the order they appear on the card.
 * Choice with options: `choose option 1, discard (Curse, Estate, Copper)`; the "options" are any of the first three forms (e.g. Cellar)
 * Choice list: `choose option 1, choose option 3` (e.g. Pawn)
 * Annotated fragments: `discard (Curse, Estate), ...`; these are used by some cards to process multiple options, and are
   optional for others to improve readability. For example, for Chapel, `trash (Curse, Copper)` and just `Curse, Copper` means the same.

In addition, a condition can be specified after the card name, e.g.
```
options for Chapel if Has(Silver, 2)
	trash (Estate, Copper)
end
options for Chapel
	trash (Estate)
end
```
can be used to trash all Estates and Coppers if your deck contains 2 silvers, but only Estates otherwise.

`reactions for` works the same, except it sometimes mentions an event name instead of a card name. Some cards (e.g. Trader)
can have both an `options for` and a `reactions for` section.
For example,
```
reactions for TorturerAttack if HasInHand(Trader, 1)
	choose option 2
end
reactions for Trader
	Curse, Copper
end
options for Trader
	Estate, Curse, Copper
end
```
can be used to choose gaining a curse when attacked by a Torturer while having a Trader in hand, and telling the Trader
to react (by gaining a Silver instead) on gaining a Curse or Copper. In addition, the Trader will be played as an action
if it can be used on an Estate, Curse or Copper, in that order.

Conditions
----------
To keep the complexity manageable, conditions have a relatively limited syntax. They start with either "if" or "if not",
and then there is exactly one (possibly nested) function call-like expression. Negations, And and Or operations can be
implemented using the Not(), AllOf(), and AnyOf() expressions, the latter two taking any number of arguments.
Conditions can always be omitted, in which case they evalute to True.

The following expressions are available:
	`AllOf(expr, expr, ...)` -- true if all of its arguments are true.
	`AnyOf(expr, expr, ...)` -- true if any of its arguments is true.
	`Not(expr)` -- true if its one argument is not true.
	`Has1InHand(Card1, Card2, ...)` -- true if you have 1 of the provided cards in your hand
	`Has2InHand(Card1, Card2, ...)` -- same for 2 cards
	`Has3InHand(Card1, Card2, ...)` -- same for 3 cards
	`HasInHand(CardName, N)` -- true if there are at least N instances of CardName in your hand
	`HasMoney(N)` -- true if at the current point in time, you have at least N virtual money. This does not include cards in your hand!
	`HasMoneyInHand(N)` -- true if the (estimated) combined value of treasure cards in your hand is at least N.
	`Has(CardName, N)` -- true if you have at least N of CardName in your deck in total
	`HasExact(CardName, N)` -- true if you have exactly N of CardName in your deck in total
	`SupplyHasLess(CardName, N)` -- true if the supply pile of CardName has less than N cards
	`SupplyEmptyPilesGreater(N)` -- true if there are more then N empty supply piles
	`HasInPlay(CardName, N)` -- true if at the current time, you have at least N instances of CardName in play (useful for e.g. Bridge)

Note that conditions not applicable to Reactions (e.g. HasMoney) will silently evalute to False in that context.

Available cards
===============
The following cards are implemented:
* Baron
* Bridge
* Cartographer
* Cellar
* Chapel
* Conspirator
* Copper
* CouncilRoom
* Courtyard
* Crossroads
* Curse
* Duchy
* Duke
* Embassy
* Estate
* Festival
* FoolsGold
* Gold
* Haggler
* Harem
* Highway
* IllGottenGains
* Ironworks
* Laboratory
* Mandarin
* Margrave
* Market
* Masquerade
* Militia
* Mine
* MiningVillage
* Minion
* Moat
* Moneylender
* Nobles
* Oasis
* Pawn
* Province
* Rebuild
* Remodel
* ShantyTown
* Shepherd
* Silver
* Smithy
* Steward
* Swindler
* ThroneRoom
* Torturer
* Torturer
* Trader
* TradingPost
* Upgrade
* Village
* Witch
* Workshop

How to add your card
====================
The stuff in cards/ should not make any assumptions about how the cards are played, I'd like that to be a completely
generic Dominion game logic library.

1.  Add the card's name to the CardId enum in cardid.h.
2.  Add the card's name to cardName() in the same file.
3.  Create a file named myamazingcard.h in gamelogic/cards/.../myamazingcard.h.
4.  Think about a card that is similar in how it works to your card. For a very simple action card, Smithy; if
    the card offers choices, Cellar or Workshop; for Attacks, Militia (enemy has a choice) or Witch (enemy has no choice);
    for a Treasure card, Ill-Gotten Gains.
5.  Copy that card's code to your file as a starting point. Adjust the name, card type, flags (if your card provides a choice,
    you must set the Choice flag), and cost.
6.  Implement playAction() or playTreasure(), or both. Override victoryPoints() if your card gives victory points.
7.  If your card provides a choice, provide an option struct. If your card is an attack, provide a react option struct.
    See the appropriate examples on how to do that.
8.  To make your card supported in the language, in genericplay.cpp, add a case to the switch handling all the cards
	in genericPlay(). Again, copy code from a similar card and adjust it accordingly.
9.  If you want, you can do the same in defaultplay.cpp to offer an option for how to play the card by default, when the
    user does not provide options for it.
10. Add your card to the supply by adding it to the factory in supply.cpp, and creating a pile for it in Supply::Supply.
11. If you like, add syntax highlighting by adding the card's name to buylist.xml.
