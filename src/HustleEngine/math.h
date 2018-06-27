#ifndef MATH_H
#define MATH_H

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(val, min, max) (((val) > (max)) ? (max) : ((val < min)) ? (min) : (val))

#endif