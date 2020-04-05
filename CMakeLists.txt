project(dominion)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

# find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(main
    src/main.cpp
    src/gamelogic/card.cpp
    src/gamelogic/cardpile.cpp
    src/gamelogic/cost.cpp
    src/gamelogic/deck.cpp
    src/gamelogic/game.cpp
    src/gamelogic/supply.cpp
    src/gamelogic/turn.cpp
    src/gamelogic/actor.cpp
)

target_include_directories(main PRIVATE src/ src/gamelogic/ src/gamelogic/cards/ src/gamelogic/actors/)
# target_link_libraries(main Qt5::Core)
