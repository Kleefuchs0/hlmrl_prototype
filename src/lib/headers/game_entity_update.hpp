#include "SpecificFloorFrictionSlowdown.hpp"
#include "SpeedVector.hpp"
#include <GameData.hpp>
#include <DebugConfiguration.hpp>
#include <entity_map_interaction.hpp>

namespace game {
    namespace loop {
        namespace update {
            void entity_floor_friction(GameData &gameData, const DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, SpecificFloorFrictionSlowdown &specificFloorFrictionSlowdown, const float frameTime);

            void entities_friction(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime);

        }
    }
}
