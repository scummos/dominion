#include "game.h"

int main() {
    for (int i = 0; i < 100000; i++) {
        Game game;
        game.run();
    }
}
