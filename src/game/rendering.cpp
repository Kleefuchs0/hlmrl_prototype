#include "game/rendering.hpp"
#include "game/ConstantRenderData.hpp"
#include "game/RenderData.hpp"
#include "lib/DebugConfiguration.hpp"
#include "raylib.h"
#include <fmt/base.h>
#include <stddef.h>

using namespace game;

void rendering::draw(ConstantRenderData &constantRenderData, DebugConfiguration &debugConfiguration, const RenderData &renderData) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(constantRenderData.renderTexture);
    BeginMode2D(renderData.cam);
    ClearBackground(renderData.backgroundColor);
    for (const RenderLayer &layer : renderData.layers) {
        for (const RenderObject &object : layer.objects) {
            fmt::println("{} {} {} {}", object.pos.x, object.pos.y, object.size.x, object.size.y);
            DrawRectanglePro({object.pos.x, object.pos.y, object.size.x, object.size.y}, {object.size.x / 2, object.size.y / 2}, object.rotation.value(), object.tint);
        }
    }
    EndMode2D();
    if (debugConfiguration.drawFPS)
        DrawFPS(10, 10);
    EndTextureMode();
    DrawTexturePro(constantRenderData.renderTexture.texture, {0, 0, static_cast<float>(constantRenderData.renderTexture.texture.width), -static_cast<float>(constantRenderData.renderTexture.texture.height)}, {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {0, 0}, 0.0f, renderData.screenTint);
    EndDrawing();
}

