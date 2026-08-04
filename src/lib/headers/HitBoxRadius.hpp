#pragma once

#include "Radius.hpp"
class HitBoxRadius : public Radius<HitBoxRadius> {
    using Radius<HitBoxRadius>::Radius;
};
