#pragma once

#include "LogLevel.hpp"
#include "raylib.h"
#include <array>
#include <cstddef>

struct DebugConfiguration {
  LogLevel logLevel = LogLevel::DEBUG;
  bool drawHitBoxes = true;
  std::array<Color, 6> avaivableHitBoxColors = {RED, {255, 0, 0, 150}, GREEN, {0, 255, 0, 150}, BLUE, {0, 0, 255, 155}};
  size_t currentHitBoxColor = 0;
  Color hitBoxColor = RED;
  bool drawFPS = true;
};
