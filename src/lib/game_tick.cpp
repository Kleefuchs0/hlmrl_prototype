#include "game_tick.hpp"
#include "DebugConfiguration.hpp"

using namespace game::loop;


void tick::tick_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
    gameData.tickClock += GetFrameTime();
    double tickTime = 1.0 / gameData.tickRate;
    if (gameData.tickClock < tickTime)
        return;

    while(gameData.tickClock >= tickTime) {
        gameData.tick++;
        ticked_function_update(gameData, debugConfiguration);
        gameData.tickClock -= tickTime;
    }
}


void tick::ticked_function_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
    for (const auto &[name, tickedEngineFunction] : gameData.tickedFunctions)
        if (gameData.tick % tickedEngineFunction.tickGoal == 0)
            tickedEngineFunction.function(gameData, debugConfiguration);

    auto entityFunctionView = gameData.registry.view<TickedFunction>();
    for (auto &entity : entityFunctionView) {
        TickedFunction &tickedFunction = gameData.registry.get<TickedFunction>(entity);
        if (gameData.tick % tickedFunction.tickGoal == 0)
            tickedFunction.function(gameData, debugConfiguration);
    }
}

