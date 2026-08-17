#pragma once
#include "lib/GameData.hpp"

namespace game {

    namespace tick {

        void ticked_function_update(GameData &gameData, DebugConfiguration &debugConfiguration);

        void tick_update(GameData &gameData, DebugConfiguration &debugConfiguration);

    }

}

