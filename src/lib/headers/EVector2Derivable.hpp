#pragma once

#include "raylib.h"
#include <cmath>

template <typename Derived>

class EVector2Derivable : public Vector2 {
    public:
     using Vector2::Vector2;
     constexpr inline EVector2Derivable() = default;
     constexpr inline EVector2Derivable(Vector2 v) : Vector2(v) {}
     constexpr inline EVector2Derivable(float x, float y)
         : Vector2{x, y} {}
     constexpr inline Derived operator +(const Vector2 &rhs) const {
         return {this->x + rhs.x, this->y + rhs.y};
     }
     constexpr inline void operator +=(const Vector2 &rhs) {
         *this = *this + rhs;
     }
     constexpr inline bool operator ==(Vector2 &rhs) const {
         return (this->x == rhs.x && this->y && rhs.y);
     }
     constexpr inline void operator =(const Vector2 &rhs) {
         this->x = rhs.x;
         this->y = rhs.y;
     }
     constexpr inline Derived operator -(const Vector2 &rhs) const {
         return {this->x - rhs.x, this->y - rhs.y};
     }
     constexpr inline Derived operator -() const {
        return {-this->x, -this->y};
     }
     constexpr inline void operator -=(const Vector2 &rhs) {
         *this = *this - rhs;
     }
     constexpr inline Derived operator *(const Vector2 &rhs) const {
         return {this->x * rhs.x, this->y * rhs.y};
     }
     constexpr inline void operator *=(const Vector2 &rhs) {
         *this = *this * rhs;
     }
     template<typename T>
     constexpr inline Derived operator *(const T &rhs) const {
         return {this->x * rhs, this->y * rhs};
     }
     template <typename T>
     constexpr inline void operator *=(const T &rhs) {
         *this = *this * rhs;
     }
     constexpr inline Derived operator /(const Vector2 &rhs) const {
         return {this->x / rhs.x, this->y / rhs.y};
     }
     constexpr inline void operator /=(const Vector2 &rhs) {
         *this = *this / rhs;
     }
     template<typename T>
     constexpr inline Derived operator /(const T &rhs) const {
         return {this->x / rhs, this->y / rhs};
     }
     template <typename T>
     constexpr inline void operator /=(const T &rhs) {
         *this = *this / rhs;
     }
          constexpr inline float sum() {
         return this->x + this->y;
     }
     constexpr inline float dotProduct(const Vector2 &rhs) const {
         return (*this * rhs).sum();
     }
     constexpr inline float angle(const Vector2 &rhs) {
         return cos(this->dotProduct(rhs));
     }
     constexpr inline float crossProduct(const Vector2 &rhs) const {
         return this->x * rhs.y - this->y * rhs.x;
     }
};

