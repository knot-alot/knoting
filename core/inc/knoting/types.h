#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace knot {

using namespace glm;

using v2 = vec2;
using v3 = vec3;
using v4 = vec4;

using v2i = vec<2, i32>;
using v2u = vec<2, u32>;
using v2d = vec<2, f64>;
using v3i = vec<3, i32>;
using v3d = vec<3, f64>;
using v4i = vec<4, i32>;
using v4d = vec<4, f64>;

using m2 = mat2;
using m3 = mat3;
using m4 = mat4;

}  // namespace knot

#define IM_VEC2_CLASS_EXTRA     \
    ImVec2(const knot::v2& f) { \
        x = f.x;                \
        y = f.y;                \
    }                           \
    operator knot::v2() const { return knot::v2(x, y); }
#define IM_VEC4_CLASS_EXTRA     \
    ImVec4(const knot::v4& f) { \
        x = f.x;                \
        y = f.y;                \
        z = f.z;                \
        w = f.w;                \
    }                           \
    operator knot::v4() const { return knot::v4(x, y, z, w); }
