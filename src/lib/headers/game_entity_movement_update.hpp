#pragma once

#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "Position.hpp"
#include "PositionMutex.hpp"
#include "SpeedVector.hpp"
#include "SpeedVectorMutex.hpp"

namespace game {
    namespace loop {
        namespace update {
            void entity_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, PositionMutex &positionMutex, SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, HitBoxRadius &hitBoxRadius, const float frameTime);

            void entities_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime);
        }
    }
}
