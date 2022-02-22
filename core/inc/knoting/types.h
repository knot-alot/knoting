#pragma once

#include <uuid.h>
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
enum class AssetType { Unknown, Texture, Mesh, Shader, Cubemap, LAST };
enum class TextureType { Albedo, Normal, Metallic, Roughness, Occlusion, LAST };

}  // namespace asset

using namespace glm;
using namespace uuids;

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

namespace glm {

template <class Archive>
void serialize(Archive& archive, glm::vec2& v) {
    archive(v.x, v.y);
}
template <class Archive>
void serialize(Archive& archive, glm::vec3& v) {
    archive(v.x, v.y, v.z);
}
template <class Archive>
void serialize(Archive& archive, glm::vec4& v) {
    archive(v.x, v.y, v.z, v.w);
}
template <class Archive>
void serialize(Archive& archive, glm::ivec2& v) {
    archive(v.x, v.y);
}
template <class Archive>
void serialize(Archive& archive, glm::ivec3& v) {
    archive(v.x, v.y, v.z);
}
template <class Archive>
void serialize(Archive& archive, glm::ivec4& v) {
    archive(v.x, v.y, v.z, v.w);
}
template <class Archive>
void serialize(Archive& archive, glm::uvec2& v) {
    archive(v.x, v.y);
}
template <class Archive>
void serialize(Archive& archive, glm::uvec3& v) {
    archive(v.x, v.y, v.z);
}
template <class Archive>
void serialize(Archive& archive, glm::uvec4& v) {
    archive(v.x, v.y, v.z, v.w);
}
template <class Archive>
void serialize(Archive& archive, glm::dvec2& v) {
    archive(v.x, v.y);
}
template <class Archive>
void serialize(Archive& archive, glm::dvec3& v) {
    archive(v.x, v.y, v.z);
}
template <class Archive>
void serialize(Archive& archive, glm::dvec4& v) {
    archive(v.x, v.y, v.z, v.w);
}

// glm matrices serialization
template <class Archive>
void serialize(Archive& archive, glm::mat2& m) {
    archive(m[0], m[1]);
}
template <class Archive>
void serialize(Archive& archive, glm::dmat2& m) {
    archive(m[0], m[1]);
}
template <class Archive>
void serialize(Archive& archive, glm::mat3& m) {
    archive(m[0], m[1], m[2]);
}
template <class Archive>
void serialize(Archive& archive, glm::mat4& m) {
    archive(m[0], m[1], m[2], m[3]);
}
template <class Archive>
void serialize(Archive& archive, glm::dmat4& m) {
    archive(m[0], m[1], m[2], m[3]);
}

template <class Archive>
void serialize(Archive& archive, glm::quat& q) {
    archive(q.x, q.y, q.z, q.w);
}
template <class Archive>
void serialize(Archive& archive, glm::dquat& q) {
    archive(q.x, q.y, q.z, q.w);
}

}  // namespace glm