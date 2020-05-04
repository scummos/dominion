#pragma once

class Event;
class Card;

bool eventGainsThisCard(Event const& event, Card const* card);
Card* cardGainedBySelf(Event const& event);
