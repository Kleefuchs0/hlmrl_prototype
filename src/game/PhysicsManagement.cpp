#include "game/PhysicsManagement.hpp"
#include "game/input_processing.hpp"
#include "game/pickups_update.hpp"
#include <algorithm>
#include <chrono>
#include <mutex>
#include "game/entity_movement_update.hpp"
#include "game/entity_fricition_update.hpp"
#include "game/rendering.hpp"

namespace game {
    namespace physics {
        void update(GameData *gameData, DebugConfiguration *debugConfiguration, RenderData *newestRenderData, bool *newestRenderDataRenewed, std::shared_mutex *newestRenderDataMutex, InputData *newestInputData, bool *newestInputDataRenewed, std::shared_mutex *newestInputDataMutex, PhysicsManagementSettings *settings) {

            InputData inputData = {};

            auto previous = std::chrono::steady_clock::now();

            double accumulator = 0.0;

            while (true) {
                {
                    std::shared_lock runningLock(gameData->runningMutex);
                    if (gameData->running == false)
                        break;
                }

                const auto now = std::chrono::steady_clock::now();

                const double frameTime = std::chrono::duration<double>(now - previous).count();

                previous = now;


                double tickTimeTarget;
                double maxAccumulatorAddition;
                {
                    std::shared_lock settingsLock(settings->settingsMutex);
                    tickTimeTarget = 1.0 / settings->tickRate;
                    maxAccumulatorAddition = settings->maxAccumulatorAddition;
                }

                accumulator += std::min(frameTime, maxAccumulatorAddition);


                while(accumulator >= tickTimeTarget) {
                    {
                        std::shared_lock newestInputDataLock(*newestInputDataMutex);
                        if (*newestInputDataRenewed == true) {
                            inputData = std::move(*newestInputData);
                            *newestInputDataRenewed = false;
                        }
                    }

                    game::input::process(*gameData, *debugConfiguration, inputData, tickTimeTarget);
                    game::physics::update_pickups(*gameData, *debugConfiguration, tickTimeTarget);
                    game::physics::update_entities_friction(*gameData, *debugConfiguration, tickTimeTarget);
                    game::physics::movement::update_entities_movement(*gameData, *debugConfiguration, tickTimeTarget);
                    {
                        std::unique_lock newestRenderDataLock(*newestRenderDataMutex);
                        *newestRenderData = game::rendering::internal::process_game_data_to_render_data(*gameData, *debugConfiguration);
                        *newestRenderDataRenewed = true;
                    }
                    accumulator -= tickTimeTarget;
                }

                const double remaining = tickTimeTarget - accumulator;

                if (remaining > 0.0)
                {
                    std::this_thread::sleep_for(
                            std::chrono::duration<double>(remaining));
                }

            }
        }
    }
}

void game::PhysicsManagement::start() {
    this->physicThread = std::jthread(game::physics::update, &gameData, &debugConfiguration, &newestRenderData, &newestRenderDataRenewed, &newestRenderDataMutex, &newestInputData, &newestInputDataRenewed, &newestInputDataMutex, &this->settings);
    this->physicThread.detach();
}
