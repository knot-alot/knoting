#pragma once

#include <knoting/px_variables_wrapper.h>
#include <uuid.h>
#include <cereal/cereal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace knot {
namespace asset {

enum class AssetState { Idle, Loading, Finished, Failed, LAST };
enum class AssetType { Unknown, Texture, Mesh, Shader, Cubemap, Script, Audio, LAST };
enum class TextureType { Albedo, Normal, Metallic, Roughness, Occlusion, LAST };
enum class SkyBoxTextureType { SkyBox, Irradiance, Radiance, LAST };

}  // namespace asset

enum class FrameBufferType : uint16_t {
    Back,
    Depth,
    Color,
    PostProcess,
    Gui,
    ShadowOne,
    ShadowTwo,
    ShadowThree,
    ShadowFour,
    LAST
};

using namespace glm;
using namespace uuids;

using vec2i = vec<2, int>;
using vec2u = vec<2, uint>;
using vec2d = vec<2, double>;
using vec3i = vec<3, int>;
using vec3d = vec<3, double>;
using vec4i = vec<4, int>;
using vec4d = vec<4, double>;

template <class Archive>
void serialize(Archive& archive, knot::vec2i& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, knot::vec2u& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, knot::vec2d& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, knot::vec3i& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, knot::vec3d& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, knot::vec4i& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}
template <class Archive>
void serialize(Archive& archive, knot::vec4d& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}

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

namespace glm {

static vec3 to_color(vec3 color) {
    return color / 255.0f;
}

template <class Archive>
void serialize(Archive& archive, glm::vec2& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, glm::vec3& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, glm::vec4& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}
template <class Archive>
void serialize(Archive& archive, glm::ivec2& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, glm::ivec3& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, glm::ivec4& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}
template <class Archive>
void serialize(Archive& archive, glm::uvec2& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, glm::uvec3& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, glm::uvec4& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}
template <class Archive>
void serialize(Archive& archive, glm::dvec2& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
}
template <class Archive>
void serialize(Archive& archive, glm::dvec3& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
}
template <class Archive>
void serialize(Archive& archive, glm::dvec4& v) {
    archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
}

// glm matrices serialization
template <class Archive>
void serialize(Archive& archive, glm::mat2& m) {
    archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]));
}
template <class Archive>
void serialize(Archive& archive, glm::dmat2& m) {
    archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]));
}
template <class Archive>
void serialize(Archive& archive, glm::mat3& m) {
    archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]), CEREAL_NVP(m[2]));
}
template <class Archive>
void serialize(Archive& archive, glm::mat4& m) {
    archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]), CEREAL_NVP(m[2]), CEREAL_NVP(m[3]));
}
template <class Archive>
void serialize(Archive& archive, glm::dmat4& m) {
    archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]), CEREAL_NVP(m[2]), CEREAL_NVP(m[3]));
}

template <class Archive>
void serialize(Archive& archive, glm::quat& q) {
    archive(CEREAL_NVP(q.x), CEREAL_NVP(q.y), CEREAL_NVP(q.z), CEREAL_NVP(q.w));
}
template <class Archive>
void serialize(Archive& archive, glm::dquat& q) {
    archive(CEREAL_NVP(q.x), CEREAL_NVP(q.y), CEREAL_NVP(q.z), CEREAL_NVP(q.w));
}

}  // namespace glm
