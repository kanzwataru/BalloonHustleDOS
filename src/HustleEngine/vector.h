#ifndef VECTOR_H
#define VECTOR_H

#include "src/hustle~1/prim.h"
#include <math.h>

static Vector2D create_vec(int x, int y) {
    Vector2D v;
    v.x = x;
    v.y = y;

    return v;
}

static Vector2D vaddv(Vector2D a, Vector2D b) {
    Vector2D v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    return v;
}

static Vector2D vsubv(Vector2D a, Vector2D b) {
    Vector2D v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    return v;
}

static Vector2D vmulv(Vector2D a, Vector2D b) {
    Vector2D v;
    v.x = a.x * b.x;
    v.y = a.y * b.y;
    return v;
}

static Vector2D vdivv(Vector2D a, Vector2D b) {
    Vector2D v;
    v.x = a.x / b.x;
    v.y = a.y / b.y;
    return v;
}

static Vector2D vaddi(Vector2D a, int val) {
    Vector2D v;
    v.x = a.x + val;
    v.y = a.y + val;
    return v;
}

static Vector2D vsubi(Vector2D a, int val) {
    Vector2D v;
    v.x = a.x - val;
    v.y = a.y - val;
    return v;
}

static Vector2D vmuli(Vector2D a, int val) {
    Vector2D v;
    v.x = a.x * val;
    v.y = a.y * val;
    return v;
}

static Vector2D vdivi(Vector2D a, int val) {
    Vector2D v;
    v.x = a.x / val;
    v.y = a.y / val;
    return v;
}

static Vector2D vnormalize(Vector2D p) {
    Vector2D v;
    v.x = (p.x > 0) - (p.x < 0);
    v.y = (p.y > 0) - (p.y < 0);
    return v;
}

static float vmagnitude(Vector2D p) {
    return sqrt((p.x * p.x) +
                (p.y * p.y));
}

static int vmagnitude_sqr(Vector2D p) {
    return ((p.x * p.x) +
            (p.y * p.y));
}

static int vdistance(Vector2D a, Vector2D b) {
    return sqrt(((b.x - a.x) * (b.x - a.x)) +
                ((b.y - a.y) * (b.y - a.y)));
}

static Point create_point(int x, int y) {
    Point v;
    v.x = x;
    v.y = y;

    return v;
}

static Point paddp(Point a, Point b) {
    Point v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    return v;
}

static Point psubp(Point a, Point b) {
    Point v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    return v;
}

static Point pmulp(Point a, Point b) {
    Point v;
    v.x = a.x * b.x;
    v.y = a.y * b.y;
    return v;
}

static Point pdivp(Point a, Point b) {
    Point v;
    v.x = a.x / b.x;
    v.y = a.y / b.y;
    return v;
}

static Point pshiftright(Point a, int shift) {
    Point v;
    v.x = a.x >> shift;
    v.y = a.y >> shift;
    return v;
}

static Point pshiftleft(Point a, int shift) {
    Point v;
    v.x = a.x << shift;
    v.y = a.y << shift;
    return v;
}

static Point paddi(Point a, int val) {
    Point v;
    v.x = a.x + val;
    v.y = a.y + val;
    return v;
}

static Point psubi(Point a, int val) {
    Point v;
    v.x = a.x - val;
    v.y = a.y - val;
    return v;
}

static Point pmuli(Point a, int val) {
    Point v;
    v.x = a.x * val;
    v.y = a.y * val;
    return v;
}

static Point pdivi(Point a, int val) {
    Point v;
    v.x = a.x / val;
    v.y = a.y / val;
    return v;
}

static Point pnormalize(Point p) {
    Point v;
    v.x = (p.x > 0) - (p.x < 0);
    v.y = (p.y > 0) - (p.y < 0);
    return v;
}

static int pmagnitude(Point p) {
    return (int)sqrt((p.x * p.x) +
                     (p.y * p.y));
}

static int pmagnitude_sqr(Point p) {
    return (int)((p.x * p.x) +
                 (p.y * p.y));
}

static int pdistance(Point a, Point b) {
    return (int)sqrt(((b.x - a.x) * (b.x - a.x)) +
                     ((b.y - a.y) * (b.y - a.y)));
}

#endif