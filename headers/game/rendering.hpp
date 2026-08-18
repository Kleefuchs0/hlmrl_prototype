#pragma once

#include "game/ConstantRenderData.hpp"
#include "game/RenderData.hpp"
#include "lib/GameData.hpp"

namespace game {

    namespace rendering {

        void draw(ConstantRenderData &constantRenderData, DebugConfiguration &debugConfiguration, const RenderData &renderData);

    }
}
