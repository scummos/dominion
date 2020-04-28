#pragma once

#include <cstring>

enum class CardId {
    Invalid,
    Copper,
    Silver,
    Gold,
    Estate,
    Duchy,
    Province,
    Smithy,
    Festival,
    Chapel,
    Cellar,
    Village,
    Workshop,
    Buerocrat,
    Militia,
    Moneylender,
    Remodel,
    ThroneRoom,
    CouncilRoom,
    Laboratory,
    Library,
    Market,
    Mine,
    Witch,
    Curse,
    Shepherd,
    Moat,
    Torturer,
    Tunnel,
    NumCards
};

static char const* cardName(CardId const id) {
    switch (id) {
        case CardId::Copper:    return "Copper";
        case CardId::Silver:    return "Silver";
        case CardId::Gold:      return "Gold";
        case CardId::Estate:    return "Estate";
        case CardId::Duchy:     return "Duchy";
        case CardId::Province:  return "Province";
        case CardId::Smithy:    return "Smithy";
        case CardId::Festival:  return "Festival";
        case CardId::Chapel:    return "Chapel";
        case CardId::Shepherd:  return "Shepherd";
        case CardId::Witch:     return "Witch";
        case CardId::Curse:     return "Curse";
        case CardId::Mine:      return "Mine";
        default:                return "Unnamed card";
    }
}

static CardId cardId(char const* name) {
    for (int i = 0; i < static_cast<int>(CardId::NumCards); i++) {
        auto const cid = static_cast<CardId>(i);
        if (!strcmp(name, cardName(cid))) {
            return cid;
        }
    }
    return CardId::Invalid;
}

