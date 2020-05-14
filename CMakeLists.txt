project(dominion)
cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Building configuration: ${CMAKE_BUILD_TYPE}")

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
    src/gamelogic/actors/genericplay.cpp
    src/gamelogic/actors/defaultplay.cpp
    src/gamelogic/actors/actor_helpers.cpp
    src/gamelogic/actors/buylistactor.cpp
    src/gamelogic/buylist/parser.cpp
    src/gamelogic/buylist/strategy.cpp
    src/gamelogic/buylist/buylist.tab.cpp
    src/gamelogic/buylist/lex.yy.cpp
    src/gamelogic/buylist/conditionfactory.cpp
)

target_include_directories(dominion PUBLIC
    src/
    src/gamelogic/
    src/gamelogic/cards/
    src/gamelogic/cards/base/
    src/gamelogic/cards/intrigue/
    src/gamelogic/cards/hinterlands/
    src/gamelogic/cards/nocturne/
    src/gamelogic/cards/darkages/
    src/gamelogic/actors/
    src/gamelogic/buylist/
)
target_link_libraries(dominion tbb)
target_compile_options(dominion PUBLIC "-Wimplicit-fallthrough")

add_executable(histogram_test src/gamelogic/tests/histogram_test.cpp)
target_link_libraries(histogram_test dominion)
add_test(histogram_test histogram_test)

add_executable(dominion_cli src/main.cpp)
target_link_libraries(dominion_cli dominion)

option(BUILD_GUI "Build the graphical user interface" ON)
if (BUILD_GUI)
    find_package(Qt5 COMPONENTS Core Gui Widgets Concurrent REQUIRED)
    find_package(Qwt REQUIRED)
    find_package(KF5SyntaxHighlighting REQUIRED)
    add_executable(dominion_qt
        src/ui/main.cpp
        src/ui/mainwindow.cpp
        src/ui/resources.qrc
    )
    target_link_libraries(dominion_qt dominion Qt5::Core Qt5::Gui Qt5::Widgets QwtLib KF5::SyntaxHighlighting Qt5::Concurrent)
endif()
