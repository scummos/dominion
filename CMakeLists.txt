project(dominion)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_library(dominion STATIC
    src/gamelogic/card.cpp
    src/gamelogic/cardpile.cpp
    src/gamelogic/cost.cpp
    src/gamelogic/deck.cpp
    src/gamelogic/game.cpp
    src/gamelogic/supply.cpp
    src/gamelogic/turn.cpp
    src/gamelogic/actor.cpp
    src/gamelogic/logger.cpp
    src/gamelogic/event.cpp
    src/gamelogic/event_helpers.cpp
    src/gamelogic/reaction.cpp
    src/gamelogic/buylist/parser.cpp
    src/gamelogic/buylist/buylist.tab.cpp
    src/gamelogic/buylist/lex.yy.cpp
    src/gamelogic/buylist/conditionfactory.cpp
)

target_include_directories(dominion PUBLIC src/ src/gamelogic/ src/gamelogic/cards/ src/gamelogic/actors/ src/gamelogic/buylist/)
target_link_libraries(dominion tbb)

add_executable(main src/main.cpp)
target_link_libraries(main dominion)

add_executable(histogram_test src/gamelogic/tests/histogram_test.cpp)
target_link_libraries(histogram_test dominion)
add_test(histogram_test histogram_test)
