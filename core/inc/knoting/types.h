#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <uuid.h>
#include <nlohmann/json.hpp>

namespace knot {

using namespace glm;
using namespace uuids;
using namespace nlohmann;

using vec2i = vec<2, i32>;
using vec2u = vec<2, u32>;
using vec2d = vec<2, f64>;
using vec3i = vec<3, i32>;
using vec3d = vec<3, f64>;
using vec4i = vec<4, i32>;
using vec4d = vec<4, f64>;

}  // namespace knot

#define IM_VEC2_CLASS_EXTRA       \
    ImVec2(const knot::vec2& f) { \
        x = f.x;                  \
        y = f.y;                  \
    }                             \
    operator knot::vec2() const { return knot::vec2(x, y); }
#define IM_VEC4_CLASS_EXTRA       \
    ImVec4(const knot::vec4& f) { \
        x = f.x;                  \
        y = f.y;                  \
        z = f.z;                  \
        w = f.w;                  \
    }                             \
    operator knot::vec4() const { return knot::vec4(x, y, z, w); }
