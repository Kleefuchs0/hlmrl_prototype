#pragma once

#include "lib/HitBoxRadius.hpp"
#include "lib/Position.hpp"
#include "lib/SpecificFloorFrictionSlowdown.hpp"
#include "lib/SpeedVector.hpp"
#include "lib/GameData.hpp"
#include "lib/DebugConfiguration.hpp"

namespace game {
    namespace physics {

        void update_entity_floor_friction(GameData &gameData, const DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, SpecificFloorFrictionSlowdown &specificFloorFrictionSlowdown, const float frameTime);

        void update_entities_friction(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime);

    }
}
