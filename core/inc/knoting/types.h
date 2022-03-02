#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <uuid.h>

namespace knot {
namespace asset {

enum class AssetState { Idle, Loading, Finished, Failed, LAST };
enum class AssetType { Unknown, Texture, Mesh, Shader, Cubemap, LAST };

enum class TextureType { Albedo, Normal, Metallic, Roughness, Occlusion, LAST };
enum class SkyBoxTextureType { Albedo, Irradiance, Radiance, LAST };

}  // namespace asset

using namespace glm;
using namespace uuids;

using vec2i = vec<2, int>;
using vec2u = vec<2, uint>;
using vec2d = vec<2, double>;
using vec3i = vec<3, int>;
using vec3d = vec<3, double>;
using vec4i = vec<4, int>;
using vec4d = vec<4, double>;

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
