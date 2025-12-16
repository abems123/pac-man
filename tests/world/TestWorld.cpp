#include <gtest/gtest.h>

#include "entities/PacMan.h"
#include "world/World.h"


TEST(WorldTest, PacManMovesRight) {
    auto* pacman = new logic::PacMan;
    pacman->setPosition(0.f, 0.f);
    pacman->setDirection(logic::Direction::Right);
    pacman->setSpeed(.2f);

    auto pos0 = pacman->getPosition();

    logic::World world;
    world.setPacman(pacman);
    world.movePacMan(logic::Direction::Right);
    world.update(0.1f);

    auto pos1 = pacman->getPosition();
    EXPECT_GT(pos1.x, pos0.x);
}
