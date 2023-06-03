#ifndef CG_ENGINE_COMMON_H
#define CG_ENGINE_COMMON_H

#include <cmath>
#include <stdexcept>
#include <cmath>
#include <array>
#include <vector>
#include <cassert>
#include <CG_Engine/math/vec2.h>
#include <CG_Engine/math/vec3.h>
#include <CG_Engine/math/vec4.h>
#include <CG_Engine/math/mat2x2.h>
#include <CG_Engine/math/mat3x3.h>
#include <CG_Engine/math/mat4x4.h>

float radians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

#endif //CG_ENGINE_COMMON_H
