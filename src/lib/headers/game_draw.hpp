#pragma once

#include "GameData.hpp"

namespace game {

    namespace loop {

        namespace draw {

            template<size_t MAP_WIDTH, size_t MAP_HEIGHT>
            void draw_map(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, GameData &gameData);

            void draw_player(GameData &gameData, DebugConfiguration &debugConfiguration);

            void draw(GameData &gameData, DebugConfiguration &debugConfiguration);
        }
    }
}
