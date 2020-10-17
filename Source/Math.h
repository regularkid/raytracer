#pragma once

#define IS_CLOSE(a, b) (std::fabs(a - b) <= FLT_EPSILON)
#define IS_ZERO(a) (std::fabs(a) <= FLT_EPSILON)
#define PI 3.14159265358979323846f
#define RAD2DEG(r) ((r * 180.0f) / PI)
#define DEG2RAD(d) ((d * PI) / 180.0f)