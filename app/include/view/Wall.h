//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_WALL_VIEW
#define PACMANPROJECT_WALL_VIEW
#include <SFML/Graphics/RectangleShape.hpp>

#include "EntityView.h"

#include <cmath>

namespace representation {

class Wall : public EntityView {
    sf::RectangleShape _shape;

    std::vector<sf::IntRect> _normal_wall_frames;
    std::vector<sf::IntRect> _edge_wall_frames;
    std::vector<sf::IntRect> _t_kruispunt_frames;
    std::vector<sf::IntRect> _tip_walls;

    bool updated = false;


public:
    explicit Wall(const std::shared_ptr<logic::EntityModel>& model);

    void onNotify(const logic::EventType& event) override;

    void updateWall();
    void update(float dt) override;

    void render(sf::RenderWindow& window) const override;

    ~Wall() override = default;

    bool compareFloats(float x, float y) { return std::fabs(x - y) < Constants::EPS; }
};
} // namespace representation

#endif // PACMANPROJECT_WALL_VIEW
