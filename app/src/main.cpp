#include <iostream>

#include "game/Game.h"

int main() {
    representation::Game game;
    game.run();

    // std::cout << logic::Stopwatch::getInstance().dt() << std::endl;
}
