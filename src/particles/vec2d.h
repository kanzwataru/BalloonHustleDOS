#ifndef BH_PRT_VEC2D_H
#define BH_PRT_VEC2D_H

#include "common/mathlib.h"

struct V2 {
    float x;
    float y;
#ifndef __cplusplus
};
#else

    inline V2() {}
    inline V2(float x_, float y_) : x(x_), y(y_) {}
    inline V2(const Point &p_) : x(p_.x), y(p_.y) {}
};

/* vector operators */
static inline V2 operator+(const V2 &a, const V2 &b) {
    V2 ret;
    ret.x = a.x + b.x;
    ret.y = a.x + b.x;
    return ret;
}

static inline void operator+=(V2 &a, const V2 &b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
}

static inline V2 operator-(const V2 &a, const V2 &b) {
    V2 ret;
    ret.x = a.x - b.x;
    ret.y = a.x - b.x;
    return ret;
}

static inline void operator-=(V2 &a, const V2 &b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
}

static inline V2 operator*(const V2 &a, const V2 &b) {
    V2 ret;
    ret.x = a.x * b.x;
    ret.y = a.x * b.x;
    return ret;
}

static inline void operator*=(V2 &a, const V2 &b) {
    a.x = a.x * b.x;
    a.y = a.y * b.y;
}

static inline V2 operator/(const V2 &a, const V2 &b) {
    V2 ret;
    ret.x = a.x / b.x;
    ret.y = a.x / b.x;
    return ret;
}

static inline void operator/=(V2 &a, const V2 &b) {
    a.x = a.x / b.x;
    a.y = a.y / b.y;
}

/* scalar operators */
template <typename T>
inline V2 operator*(const V2 &a, const T &b) { \
    V2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    return ret;
}

template <typename T>
inline void operator*=(V2 &a, const T &b) { \
    a.x = a.x * b;
    a.y = a.y * b;
}

template <typename T>
inline V2 operator/(const V2 &a, const T &b) { \
    V2 ret;
    ret.x = a.x / b;
    ret.y = a.y / b;
    return ret;
}

template <typename T>
inline void operator/=(V2 &a, const T &b) { \
    a.x = a.x /= b;
    a.y = a.y /= b;
}

template <typename T>
inline V2 operator+(const V2 &a, const T &b) { \
    V2 ret;
    ret.x = a.x + b;
    ret.y = a.y + b;
    return ret;
}

template <typename T>
inline void operator+=(V2 &a, const T &b) { \
    a.x = a.x += b;
    a.y = a.y += b;
}

template <typename T>
inline V2 operator-(const V2 &a, const T &b) { \
    V2 ret;
    ret.x = a.x - b;
    ret.y = a.y - b;
    return ret;
}

template <typename T>
inline void operator-=(V2 &a, const T &b) { \
    a.x = a.x -= b;
    a.y = a.y -= b;
}

#endif /* c++ */
#endif /* guard */
