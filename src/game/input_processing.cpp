#include "game/input_processing.hpp"
#include "lib/Acceleration.hpp"
#include "lib/BodyRotation.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/PlayerMarker.hpp"
#include "lib/SpeedVector.hpp"
#include <cmath>
#include <fmt/base.h>

using namespace game;

namespace game {
    namespace input {
        namespace internal {
            namespace singular {
                void apply_to_player(SpeedVector &speedVector, const Acceleration &acceleration, const EVector2 &change, const Position &playerPosition , BodyRotation &playerRotation, const Position &cursorPosition, const float deltaTime) {
                    speedVector += change.normalized() * acceleration.value() * deltaTime;
                    Position relativePosition = playerPosition - cursorPosition;
                    playerRotation = std::atan2(relativePosition.y, relativePosition.x) * static_cast<float>(180 / M_PI);
                }
            }
            void apply_to_players(GameData &gameData, const InputData &inputData, const float deltaTime) {
                const auto& playerView = gameData.registry.view<PlayerMarker, SpeedVector, Acceleration, Position, BodyRotation>();
                for (const entt::entity &player : playerView) {
                    const auto &[speedVector, acceleration, position, rotation] = playerView.get<SpeedVector, Acceleration, Position, BodyRotation>(player);
                    singular::apply_to_player(speedVector, acceleration, inputData.relativeMovement, position, rotation, inputData.cursorPosition, deltaTime);
                }
            }
        }
    }
}

void input::process(GameData &gameData, [[maybe_unused]] const DebugConfiguration &DebugConfiguration, const InputData &inputData, const float deltaTime) {
    internal::apply_to_players(gameData, inputData, deltaTime);
}
