#pragma once

#include "game/RenderObject.hpp"
#include <vector>

struct RenderLayer {
    std::vector<RenderObject> objects;
};
