#pragma once

#include "game/ConstantRenderData.hpp"
#include "game/RenderData.hpp"
#include "lib/GameData.hpp"

namespace game {

    namespace rendering {

        namespace internal {
            RenderData process_game_data_to_render_data(GameData &gameData, DebugConfiguration &debugConfiguration);
        }

        void draw(ConstantRenderData &constantRenderData, DebugConfiguration &debugConfiguration, const RenderData &renderData);

    }
}
