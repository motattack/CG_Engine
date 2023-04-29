#ifndef CG_ENGINE_COMMON_H
#define CG_ENGINE_COMMON_H

#include <cmath>

float radians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

#endif //CG_ENGINE_COMMON_H
