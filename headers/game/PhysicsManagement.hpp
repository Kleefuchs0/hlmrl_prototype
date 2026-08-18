#pragma once

#include "game/RenderData.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/GameData.hpp"
#include <cstdint>
#include <thread>

namespace game {

    class PhysicsManagementSettings {
        public:
            std::shared_mutex settingsMutex;
            uint32_t tickRate = 320;
            double maxAccumulatorAddition = 0.25;
            PhysicsManagementSettings(uint32_t tickRate) : tickRate(tickRate) {
                maxAccumulatorAddition = static_cast<double>(tickRate) / 10;
            }
    };

    void update(GameData *gameData, DebugConfiguration *debugConfiguration, RenderData *newestRenderData, bool *newestRenderDataRenewed, std::shared_mutex *newestRenderDataMutex, PhysicsManagementSettings *settings);

    class PhysicsManagement {
        private:
            GameData &gameData;
            DebugConfiguration &debugConfiguration;
            PhysicsManagementSettings settings;
            RenderData &newestRenderData;
            bool &newestRenderDataRenewed;
            std::shared_mutex &newestRenderDataMutex;
            std::jthread physicThread;
        public:
            PhysicsManagement(GameData &gameData, DebugConfiguration &debugConfiguration, RenderData &newestRenderData, bool &newestRenderDataRenewed, std::shared_mutex &newestRenderDataMutex, const uint32_t tickRate) : gameData(gameData), debugConfiguration(debugConfiguration), settings(tickRate), newestRenderData(newestRenderData), newestRenderDataRenewed(newestRenderDataRenewed), newestRenderDataMutex(newestRenderDataMutex) {
            }

            void start();
    };

}
