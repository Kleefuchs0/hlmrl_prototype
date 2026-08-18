#include "game/input_processing.hpp"
#include "lib/Acceleration.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/PlayerMarker.hpp"
#include "lib/SpeedVector.hpp"

using namespace game;

namespace game {
    namespace input {
        namespace internal {
            namespace singular {
                void apply_to_player(SpeedVector &speedVector, const Acceleration &acceleration, const EVector2 &change, const float deltaTime) {
                    speedVector += change * acceleration.value() * deltaTime;
                }
            }
            void apply_to_players(GameData &gameData, const InputData &inputData, const float deltaTime) {
                const auto& playerView = gameData.registry.view<PlayerMarker, SpeedVector, Acceleration>();
                for (const entt::entity &player : playerView) {
                    const auto &[speedVector, acceleration] = playerView.get<SpeedVector, Acceleration>(player);
                    singular::apply_to_player(speedVector, acceleration, inputData.relativeMovement, deltaTime);
                }
            }
        }
    }
}

void input::process(GameData &gameData, [[maybe_unused]] const DebugConfiguration &DebugConfiguration, const InputData &inputData, const float deltaTime) {
    internal::apply_to_players(gameData, inputData, deltaTime);
}
