#pragma once

#include "raylib.h"

class ConstantRenderData {
    public:
        RenderTexture2D renderTexture;
        int worldWidth;
        int worldHeight;
        ConstantRenderData(int worldWidth, int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight) {
            this->renderTexture = LoadRenderTexture(worldWidth, worldHeight);
        }

        ~ConstantRenderData() {
            UnloadRenderTexture(this->renderTexture);
        }
};

