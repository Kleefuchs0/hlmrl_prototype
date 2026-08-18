#pragma once

#include "game/TextureFrame.hpp"
#include "lib/BodyRotation.hpp"
#include "lib/BodySize.hpp"
#include "lib/Position.hpp"

struct RenderObject {
    TextureFrame textureFrame;
    Color tint;
    Position pos;
    BodySize size;
    BodyRotation rotation;
};
