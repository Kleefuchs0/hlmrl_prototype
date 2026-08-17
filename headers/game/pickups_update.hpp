#pragma once

#include "lib/BodyRotation.hpp"
#include "lib/BodyRotationMutex.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/GameData.hpp"

namespace game {
    namespace physics {
        namespace singular {
            void update_pickup(BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const float frameTime);
        }

        void update_pickups(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, const float frameTime);
    }
}
