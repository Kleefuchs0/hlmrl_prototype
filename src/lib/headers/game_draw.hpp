#pragma once

#include "BodyRotation.hpp"
#include "BodySize.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "Position.hpp"

namespace game {

    namespace loop {

        namespace draw {

            template<size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
            void draw_map(Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, GameData &gameData);
            void draw_entities(GameData &gameData, DebugConfiguration &debugConfiguration);

            void draw_entity(const GameData &gameDate, const DebugConfiguration &debugConfiguration, const Position &position, const BodySize &bodySize, const BodyRotation &bodyRotation, const HitBoxRadius &hitBoxRadius);

            void draw(GameData &gameData, DebugConfiguration &debugConfiguration);
        }
    }
}
