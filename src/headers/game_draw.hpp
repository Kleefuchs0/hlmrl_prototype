#pragma once

#include "GameData.hpp"

namespace game {

    namespace loop {

        namespace draw {

            void draw_map(const GameData &gameData);

            void draw_player(GameData &gameData);

            void draw(GameData &gameData);
        }
    }
}
