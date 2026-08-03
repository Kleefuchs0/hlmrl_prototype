#include "game_tick.hpp"

using namespace game::loop;


void tick::tick_update(GameData &gameData) {
    gameData.tickClock += GetFrameTime();
    double tickTime = 1.0 / gameData.tickRate;
    if (gameData.tickClock < tickTime) {
        return;
    }
    while(gameData.tickClock >= tickTime) {
        gameData.tick++;
        ticked_function_update(gameData);
        gameData.tickClock -= tickTime;
    }
}


void tick::ticked_function_update(GameData &gameData) {
    for (auto it = gameData.tickedFunctions.begin(); it != gameData.tickedFunctions.end(); it++) {
        TickedFunction &tickedEngineFunction = it->second;
        if (gameData.tick % tickedEngineFunction.tickGoal == 0) {
            tickedEngineFunction.function(gameData);
        }
    }
}

