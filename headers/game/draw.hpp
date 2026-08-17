#pragma once

#include "lib/BodyRotation.hpp"
#include "lib/BodyRotationMutex.hpp"
#include "lib/BodySize.hpp"
#include "lib/GameData.hpp"
#include "lib/HitBoxRadius.hpp"
#include "lib/Position.hpp"
#include "lib/PositionMutex.hpp"

namespace game {

    namespace loop {

        namespace draw {

            template<size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
            void draw_map(Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, GameData &gameData);
            void draw_entities(GameData &gameData, DebugConfiguration &debugConfiguration);

            void draw_entity(const GameData &gameDate, const DebugConfiguration &debugConfiguration, const Position &position, PositionMutex &positionMutex, const BodySize &bodySize, const BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const HitBoxRadius &hitBoxRadius);

            void draw(GameData &gameData, DebugConfiguration &debugConfiguration);
        }
    }
}
