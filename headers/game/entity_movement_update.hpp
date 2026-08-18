#pragma once

#include "lib/GameData.hpp"
#include "lib/HitBoxRadius.hpp"
#include "lib/Position.hpp"
#include "lib/SpeedVector.hpp"

namespace game {
    namespace physics {
        namespace movement {
            namespace singular {
                void update_entity_movement(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, const float frameTime);
            }

            void update_entities_movement(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime);
        }
    }
}
