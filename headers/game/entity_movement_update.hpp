#pragma once

#include "lib/GameData.hpp"

namespace game {
    namespace physics {
        namespace movement {
            void update_entities_movement(GameData &gameData, const DebugConfiguration &debugConfiguration, const float frameTime);
        }
    }
}
