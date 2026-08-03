#include "HitBoxes.hpp"
#include "Position.hpp"
#include <cmath>
#include <array>

bool GetCollisionHitBoxes(const HitBoxRectangle hbr1, const HitBoxRectangle hbr2) {
    std::array<Position, 4> vertices1;
    vertices1[0] = {(hbr1.pos.x - hbr1.size.x / 2) * std::cos(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::sin(hbr1.rotation), (hbr1.pos.x - hbr1.size.x / 2) * std::sin(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::cos(hbr1.rotation)};
    vertices1[1] = {(hbr1.pos.x + hbr1.size.x / 2) * std::cos(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::sin(hbr1.rotation), (hbr1.pos.x - hbr1.size.x / 2) * std::sin(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::cos(hbr1.rotation)};
    vertices1[2] = {(hbr1.pos.x - hbr1.size.x / 2) * std::cos(hbr1.rotation) - (hbr1.pos.y + hbr1.size.y / 2) * std::sin(hbr1.rotation), (hbr1.pos.x - hbr1.size.x / 2) * std::sin(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::cos(hbr1.rotation)};
    vertices1[3] = {(hbr1.pos.x + hbr1.size.x / 2) * std::cos(hbr1.rotation) - (hbr1.pos.y + hbr1.size.y / 2) * std::sin(hbr1.rotation), (hbr1.pos.x - hbr1.size.x / 2) * std::sin(hbr1.rotation) - (hbr1.pos.y - hbr1.size.y / 2) * std::cos(hbr1.rotation)};
}
