//
// Created by abdellah on 12/13/25.
//

#include <iostream>

#include "utils/Stopwatch.h"

int main() {
    auto& sw = logic::Stopwatch::getInstance();

    // simulate game loop
    for (int i = 0; i < 500; i++) {
        sw.tick();
        std::cout << "dt = " << sw.dt() << " seconds" << std::endl;
    }
}
