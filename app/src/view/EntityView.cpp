//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/EntityView.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "camera/Camera.h"
#include "entities/EntityModel.h"


namespace representation {
    void EntityView::setFrames(const std::vector<sf::IntRect> &frames) {
        _frames = frames;
        _current_frame = 0;
        if (!_frames.empty()) {
            _sprite.setTextureRect(_frames[0]);
        }
    }

    EntityView::EntityView(const std::shared_ptr<logic::EntityModel> &model, const sf::Texture &tex,
                           const Camera &camera) : _model(model), _texture(tex), _camera(camera) {
        _sprite.setTexture(_texture);

    }

    EntityView::~EntityView() = default;

    void EntityView::update(float dt) {
        // animation…
        if (!_frames.empty()) {
            _animation_timer += dt;

            if (_animation_timer >= _frame_duration) {
                _animation_timer = 0.f;
                _current_frame = (_current_frame + 1) % _frames.size();
                _sprite.setTextureRect(_frames[_current_frame]);
            }
        }

        // update position:
        // auto pos = _model->getPosition();
        // _sprite.setPosition(_camera.worldToScreen(pos.first, pos.second));
    }


    void EntityView::render(sf::RenderWindow &window) const {
        window.draw(_sprite);
    }
}
