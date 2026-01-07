//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "entities/EntityModel.h"

namespace logic {
class Collectable : public EntityModel {
  int score;

public:
  Collectable(const float x, const float y, const int score, logic::EntityType type)
      : EntityModel(x, y, type), score(score) {}

  [[nodiscard]] int getScore() const { return score; }

};
}
