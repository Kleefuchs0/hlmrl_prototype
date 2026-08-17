#include "game/player_cam_update.hpp"
#include "lib/PlayerMarker.hpp"
#include "lib/Position.hpp"
#include "lib/PositionMutex.hpp"
#include <mutex>

using namespace game;

void physics::update_players_cam(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto playerView = gameData.registry.view<PlayerMarker, Position, PositionMutex>();
    for (const entt::entity &player : playerView) {
        const auto &[position, positionMutex] = gameData.registry.get<Position, PositionMutex>(player);
        std::shared_lock positionLock(positionMutex);
        std::unique_lock camLock(gameData.camMutex);
        gameData.cam.target = position;
    }
}
