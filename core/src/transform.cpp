#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/transform.h>

namespace knot {
namespace components {

Transform::Transform(const vec3& position, const vec3& scale, const quat& rotation)
    : m_position(position),
      m_scale(scale),
      m_rotation(rotation),
      m_isDirty(true),
      m_modelMatrix(1.0f),
      m_parentModelMatrix(1.0f) {}

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
    m_isDirty = true;
}

void Transform::set_scale(const vec3& scale) {
    m_scale = scale;
    m_isDirty = true;
}

void Transform::set_rotation(const quat& rotation) {
    m_rotation = rotation;
    m_isDirty = true;
}

void Transform::set_rotation_euler(const vec3& euler) {
    m_rotation = quat(radians(euler));
    m_isDirty = true;
}

mat4 Transform::get_model_matrix_internal() const {
    mat4 modelMatrix = mat4(1.0f);
    modelMatrix = translate(mat4(1.0f), m_position) * toMat4(m_rotation) * scale(mat4(1.0f), m_scale);
    return modelMatrix;
}

mat4 Transform::get_model_matrix() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return m_modelMatrix;

    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    entt::entity goHandle = entt::to_entity(registry, *this);

    auto goOpt = scene.get_game_object_from_handle(goHandle);
    if (!goOpt)
        return m_modelMatrix;

    GameObject go = goOpt.value();
    Hierarchy& hierarchy = go.get_component<Hierarchy>();

    if (!hierarchy.has_parent()) {
        if (!m_isDirty)
            return m_modelMatrix;

        m_modelMatrix = get_model_matrix_internal();
        m_isDirty = false;
        return m_modelMatrix;
    }

    std::optional<uuid> parentUUID = hierarchy.get_parent();
    if (!parentUUID)
        return m_modelMatrix;

    std::optional<GameObject> parentOpt = scene.get_game_object_from_id(parentUUID.value());
    if (!parentOpt)
        return m_modelMatrix;

    GameObject parent = parentOpt.value();
    Transform& parentTransform = parent.get_component<Transform>();

    if (!parentTransform.m_isDirty && !m_isDirty)
        return m_modelMatrix;

    m_parentModelMatrix = parentTransform.get_model_matrix();
    m_modelMatrix = m_parentModelMatrix * get_model_matrix_internal();
    m_isDirty = false;

    return m_modelMatrix;
}

mat4 Transform::get_parent_model_matrix() const {
    return m_parentModelMatrix;
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
