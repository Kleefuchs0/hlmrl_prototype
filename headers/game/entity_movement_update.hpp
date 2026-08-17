#pragma once

#include "lib/GameData.hpp"
#include "lib/HitBoxRadius.hpp"
#include "lib/Position.hpp"
#include "lib/PositionMutex.hpp"
#include "lib/SpeedVector.hpp"
#include "lib/SpeedVectorMutex.hpp"

namespace game {
    namespace physics {
        namespace movement {
            namespace singular {
                void update_entity_movement(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, PositionMutex &positionMutex, SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, HitBoxRadius &hitBoxRadius, const float frameTime);
            }

            void update_entities_movement(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime);
        }
    }
}
