#pragma once

template <typename Derived, typename T> 
class Value {
    private:
     T value_ = 0;
    public:
     constexpr inline Value() = default;
     constexpr inline Value(T v) : value_(v) {};
     constexpr inline T &value() {
         return this->value_;
     }
     const constexpr inline T &value() const {
         return this->value_;
     }
     constexpr inline auto operator <=>(const Derived &rhs) const {
         return this->value_ <=> rhs.value();
     }
     constexpr inline bool operator ==(const Derived &rhs) const {
         return this->value_ == rhs.value();
     }
     constexpr inline Derived operator +(const Derived &rhs) const {
         return Derived(this->value_ + rhs.value());
     }
     template <typename rhsT>
     constexpr inline Derived operator +(const rhsT &rhs) const {
         return Derived(this->value_ + rhs);
     }
     constexpr inline void operator +=(const Derived &rhs) {
         *this = *this + rhs;
     }
     template <typename rhsT>
     constexpr inline void operator +=(const rhsT &rhs) {
         *this = *this + rhs;
     }
     constexpr inline Derived operator -(const Derived &rhs) const {
         return Derived(this->value_ - rhs.value());
     }
     template <typename rhsT>
     constexpr inline Derived operator -(const rhsT &rhs) const {
         return Derived(this->value_ - rhs);
     }
     constexpr inline void operator -=(const Derived &rhs) {
         *this = *this - rhs;
     }
     template <typename rhsT>
     constexpr inline void operator -=(const rhsT &rhs) {
         *this = *this - rhs;
     }
     constexpr inline Derived operator -() const {
         return Derived(-this->value_);
     }
     constexpr inline Derived operator *(const Derived &rhs) const {
         return Derived(this->value_ * rhs.value());
     }
     template <typename rhsT>
     constexpr inline Derived operator *(const rhsT &rhs) const {
         return Derived(this->value_ * rhs);
     }
     constexpr inline void operator *=(const Derived &rhs) {
        *this = *this * rhs;
     }
     template <typename rhsT>
     constexpr inline void operator *=(const rhsT &rhs) {
         *this = *this * rhs;
     }
     constexpr inline Derived operator /(const Derived &rhs) const {
         return Derived(this->value_ / rhs.value());
     }
     template <typename rhsT>
     constexpr inline Derived operator /(const rhsT &rhs) const {
         return Derived(this->value_ / rhs);
     }
     constexpr inline void operator /=(const Derived &rhs) {
         *this = *this / rhs;
     }
     template <typename rhsT>
     constexpr inline void operator /=(const rhsT &rhs) {
         *this = *this / rhs;
     }
};



