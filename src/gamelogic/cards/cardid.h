#pragma once

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
        default:                return "Unnamed card";
    }
}

