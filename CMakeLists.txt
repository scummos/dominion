project(dominion)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

add_executable(main
    src/main.cpp
    src/gamelogic/card.cpp
    src/gamelogic/cardpile.cpp
    src/gamelogic/cost.cpp
    src/gamelogic/deck.cpp
    src/gamelogic/game.cpp
    src/gamelogic/supply.cpp
    src/gamelogic/turn.cpp
)

target_include_directories(main PRIVATE src/ src/gamelogic/ src/gamelogic/cards/ src/gamelogic/actors/)
