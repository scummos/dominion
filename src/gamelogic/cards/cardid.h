#pragma once

enum class CardId {
    Copper,
    Silver,
    Gold,
    Estate,
    Duchy,
    Province,
    Smithy,
    Festival,
    Chapel,
    Shepherd,
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

