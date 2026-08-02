#pragma once

#include "raylib.h"
#include <cmath>

class EVector2 : public Vector2 {
    public:
     using Vector2::Vector2;
     constexpr inline EVector2() = default;
     constexpr inline EVector2(float x, float y)
         : Vector2{x, y} {}
     constexpr inline EVector2 operator +(const Vector2 &rhs) const {
         return {this->x + rhs.x, this->y + rhs.y};
     }
     constexpr inline void operator +=(const Vector2 &rhs) {
         *this = *this + rhs;
     }
     constexpr inline bool operator ==(const Vector2 &rhs) const {
         return (this->x == rhs.x && this->y && rhs.y);
     }
     constexpr inline void operator =(const Vector2 &rhs) {
         this->x = rhs.x;
         this->y = rhs.y;
     }
     constexpr inline EVector2 operator -(const Vector2 &rhs) const {
         return {this->x - rhs.x, this->y - rhs.y};
     }
     constexpr inline EVector2 operator -() const {
        return {-this->x, -this->y};
     }
     constexpr inline void operator -=(const Vector2 &rhs) {
         *this = *this - rhs;
     }
     constexpr inline EVector2 operator *(const Vector2 &rhs) const {
         return {this->x * rhs.x, this->y * rhs.y};
     }
     constexpr inline void operator *=(const Vector2 &rhs) {
         *this = *this * rhs;
     }
     template<typename T>
     constexpr inline EVector2 operator *(const T &rhs) const {
         return {this->x * rhs, this->y * rhs};
     }
     template <typename T>
     constexpr inline void operator *=(const T &rhs) {
         *this = *this * rhs;
     }
     constexpr inline EVector2 operator /(const Vector2 &rhs) const {
         return {this->x / rhs.x, this->y / rhs.y};
     }
     constexpr inline void operator /=(const Vector2 &rhs) {
         *this = *this / rhs;
     }
     template<typename T>
     constexpr inline EVector2 operator /(const T &rhs) const {
         return {this->x / rhs, this->y / rhs};
     }
     template <typename T>
     constexpr inline void operator /=(const T &rhs) {
         *this = *this / rhs;
     }
};

