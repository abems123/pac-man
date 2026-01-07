#include "game/Game.h"

int main() {
    try {
        representation::Game game;
        game.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Fatal unknown error.\n";
        return 2;
    }
}
