#include <knoting/log.h>
#include <knoting/transform.h>

namespace knot {
namespace components {

Transform::Transform(const vec3& position, const vec3& scale, const quat& rotation)
    : m_position(position), m_scale(scale), m_rotation(rotation) {}

void Transform::on_awake() {}
void Transform::on_destroy() {}

vec3 Transform::get_position() const {
    return m_position;
}

vec3 Transform::get_scale() const {
    return m_scale;
}

quat Transform::get_rotation() const {
    return m_rotation;
}

vec3 Transform::get_rotation_euler() const {
    return degrees(eulerAngles(m_rotation));
}

void Transform::set_position(const vec3& position) {
    m_position = position;
}

void Transform::set_scale(const vec3& scale) {
    m_scale = scale;
}

void Transform::set_rotation(const quat& rotation) {
    m_rotation = rotation;
}

void Transform::set_rotation_euler(const vec3& euler) {
    m_rotation = quat(radians(euler));
}

glm::mat4 Transform::get_model_matrix() const {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation) * glm::scale(mat4(1.0f), m_scale);
    return modelMatrix;
}
vec3 Transform::forward() const {
    constexpr vec3 forward = vec3(0, 0, 1);
    return m_rotation * forward;
}
vec3 Transform::up() const {
    constexpr vec3 up = vec3(0, 1, 0);
    return m_rotation * up;
}
vec3 Transform::right() const {
    constexpr vec3 right = vec3(1, 0, 0);
    return m_rotation * right;
}

}  // namespace components
}  // namespace knot
