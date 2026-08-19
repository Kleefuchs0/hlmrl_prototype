#include "game/entity_movement_update.hpp"
#include "lib/Acceleration.hpp"
#include "lib/MaxSpeed.hpp"
#include "lib/entity_try_move.hpp"
#include <fmt/base.h>

using namespace game;

namespace game {
    namespace physics {
        namespace movement {
            namespace internal {
                namespace singular {

                    template <size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
                    void update_entity_position(const Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, const float frameTime) {
                        SpeedVector change = speedVector * frameTime;
                        try_move_entity_with_deltaSpeed_change_on_collision(position, speedVector, hitBoxRadius, map, change);
                    }

                    void update_entity_cap_speed(SpeedVector &speedVector, const MaxSpeed &maxSpeed) {
                        if (speedVector.x > maxSpeed) {
                            speedVector.x = maxSpeed.value();
                        } else if (speedVector.x < -maxSpeed) {
                            speedVector.x = -maxSpeed.value();
                        }
                        if (speedVector.y > maxSpeed) {
                            speedVector.y = maxSpeed.value();
                        } else if (speedVector.y < -maxSpeed) {
                            speedVector.y = -maxSpeed.value();
                        }
                    }
                }
                void update_entities_position(GameData &gameData, const float deltaTime) {
                    auto entityView = gameData.registry.view<Position, SpeedVector,  Acceleration, HitBoxRadius>();
                    for (const entt::entity &entity : entityView) {
                        const auto &[position, speedVector, hitBoxRadius] = entityView.get<Position, SpeedVector, HitBoxRadius>(entity);
                        singular::update_entity_position(gameData.map, position, speedVector, hitBoxRadius, deltaTime);
                    }
                }

                void update_entities_cap_speed(GameData &gameData) {
                    auto entityView = gameData.registry.view<SpeedVector, MaxSpeed>();
                    for (const entt::entity &entity : entityView) {
                        const auto &[speedVector, maxSpeed] = entityView.get<SpeedVector, MaxSpeed>(entity);
                        singular::update_entity_cap_speed(speedVector, maxSpeed);
                    }
                }
            }
        }
    }
}

void physics::movement::update_entities_movement(GameData &gameData, [[maybe_unused]] const DebugConfiguration &debugConfiguration, const float deltaTime) {
    internal::update_entities_cap_speed(gameData);
    internal::update_entities_position(gameData, deltaTime);
}
