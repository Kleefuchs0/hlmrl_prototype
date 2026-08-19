#pragma once
#include "RenderLayer.hpp"

struct RenderData {
    std::vector<RenderLayer> layers;
    RenderLayer mapLayer;
    Color screenTint = WHITE;
    Color backgroundColor = BLUE;
    int worldWidth = 0;
    int worldHeight = 0;
    Camera2D cam;
};

