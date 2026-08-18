#include "game/PhysicsManagement.hpp"
#include "game/pickups_update.hpp"
#include <algorithm>
#include <chrono>
#include <mutex>
#include "game/entity_movement_update.hpp"
#include "game/entity_fricition_update.hpp"
#include "game/rendering.hpp"

void game::update(GameData *gameData, DebugConfiguration *debugConfiguration, RenderData *newestRenderData, bool *newestRenderDataRenewed, std::shared_mutex *newestRenderDataMutex, PhysicsManagementSettings *settings) {
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
};

void game::PhysicsManagement::start() {
    this->physicThread = std::jthread(update, &gameData, &debugConfiguration, &newestRenderData, &newestRenderDataRenewed, &newestRenderDataMutex, &this->settings);
    this->physicThread.detach();
}
