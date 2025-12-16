//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Wall.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "camera/Camera.h"

namespace representation {
    Wall::Wall(const std::shared_ptr<logic::EntityModel> &model, const sf::Texture &,
               const Camera &camera) : EntityView(model, sf::Texture(), camera) {

        _shape.setFillColor(sf::Color::Blue);

        auto pos = model->getPosition();


        auto screen_position = camera.worldToScreen(pos.first, pos.second);
        const sf::Vector2f screen_size = {
            camera.getTileWidth(), camera.getTileHeight()
        };

        std::cout << "============================================" << std::endl;
        std::cout << "Tile Height in Wall using Camera = " << screen_size.x << std::endl;
        std::cout << "Tile Width in Wall using Camera =  " << screen_size.y << std::endl;
        std::cout << "============================================" << std::endl;

        _shape.setPosition(screen_position);
        _shape.setSize(screen_size);
    }

    void Wall::onNotify(const logic::EventType &event) {
    }

    void Wall::update(float dt) {
        // _shape.setSize({_camera.getTileWidth(_model->getWidth()), _camera.getTileWidth(_model->getHeight())});
    }

    void Wall::render(sf::RenderWindow &window) const {
        window.draw(_shape);
    }
}
