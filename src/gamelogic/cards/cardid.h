#pragma once

#include <string>

enum class CardId {
    Invalid,
    NoCard,
    Baron,
    Bridge,
    Buerocrat,
    Cartographer,
    Cellar,
    Chapel,
    Conspirator,
    Copper,
    CouncilRoom,
    Courtyard,
    Crossroads,
    Curse,
    Duchy,
    Duke,
    Embassy,
    Estate,
    Festival,
    FoolsGold,
    Gold,
    Haggler,
    Harem,
    Highway,
    IllGottenGains,
    Ironworks,
    Laboratory,
    Library,
    Mandarin,
    Margrave,
    Market,
    Masquerade,
    Militia,
    Mine,
    MiningVillage,
    Minion,
    Moat,
    Moneylender,
    Nobles,
    Oasis,
    Pawn,
    Province,
    Rebuild,
    Remodel,
    ShantyTown,
    Shepherd,
    Silver,
    Smithy,
    Steward,
    Swindler,
    ThroneRoom,
    Torturer,
    Trader,
    TradingPost,
    Tunnel,
    Upgrade,
    Village,
    WishingWell,
    Witch,
    Workshop,
    NumCards /* don't move me */
};

static char const* cardName(CardId const id) {
    switch (id) {
        case CardId::NoCard:         return "Nothing";
        case CardId::Baron:          return "Baron";
        case CardId::Bridge:         return "Bridge";
        case CardId::Buerocrat:      return "Buerocrat";
        case CardId::Cartographer:   return "Cartographer";
        case CardId::Cellar:         return "Cellar";
        case CardId::Chapel:         return "Chapel";
        case CardId::Conspirator:    return "Conspirator";
        case CardId::Copper:         return "Copper";
        case CardId::CouncilRoom:    return "CouncilRoom";
        case CardId::Courtyard:      return "Courtyard";
        case CardId::Crossroads:     return "Crossroads";
        case CardId::Curse:          return "Curse";
        case CardId::Duchy:          return "Duchy";
        case CardId::Duke:           return "Duke";
        case CardId::Embassy:        return "Embassy";
        case CardId::Estate:         return "Estate";
        case CardId::Festival:       return "Festival";
        case CardId::FoolsGold:      return "FoolsGold";
        case CardId::Gold:           return "Gold";
        case CardId::Haggler:        return "Haggler";
        case CardId::Harem:          return "Harem";
        case CardId::Highway:        return "Highway";
        case CardId::IllGottenGains: return "IllGottenGains";
        case CardId::Ironworks:      return "Ironworks";
        case CardId::Laboratory:     return "Laboratory";
        case CardId::Library:        return "Library";
        case CardId::Mandarin:       return "Mandarin";
        case CardId::Margrave:       return "Margrave";
        case CardId::Market:         return "Market";
        case CardId::Masquerade:     return "Masquerade";
        case CardId::Militia:        return "Militia";
        case CardId::Mine:           return "Mine";
        case CardId::MiningVillage:  return "MiningVillage";
        case CardId::Minion:         return "Minion";
        case CardId::Moat:           return "Moat";
        case CardId::Moneylender:    return "Moneylender";
        case CardId::Nobles:         return "Nobles";
        case CardId::Oasis:          return "Oasis";
        case CardId::Pawn:           return "Pawn";
        case CardId::Province:       return "Province";
        case CardId::Rebuild:        return "Rebuild";
        case CardId::Remodel:        return "Remodel";
        case CardId::ShantyTown:     return "ShantyTown";
        case CardId::Shepherd:       return "Shepherd";
        case CardId::Silver:         return "Silver";
        case CardId::Smithy:         return "Smithy";
        case CardId::Steward:        return "Steward";
        case CardId::Swindler:       return "Swindler";
        case CardId::ThroneRoom:     return "ThroneRoom";
        case CardId::Torturer:       return "Torturer";
        case CardId::Trader:         return "Trader";
        case CardId::TradingPost:    return "TradingPost";
        case CardId::Upgrade:        return "Upgrade";
        case CardId::Village:        return "Village";
        case CardId::WishingWell:    return "WishingWell";
        case CardId::Witch:          return "Witch";
        case CardId::Workshop:       return "Workshop";
        default:                     return "Unnamed card";
    }
}

static CardId cardId(std::string name) {
    for (int i = 0; i < static_cast<int>(CardId::NumCards); i++) {
        auto const cid = static_cast<CardId>(i);
        if (name == cardName(cid)) {
            return cid;
        }
    }
    return CardId::Invalid;
}

