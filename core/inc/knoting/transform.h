#pragma once

#include <knoting/log.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>

namespace knot {
namespace components {

class Transform {
   public:
    Transform(const vec3& position = vec3(0.0f),
              const vec3& scale = vec3(1.0f),
              const quat& rotation = quat(1.0f, 0.0f, 0.0f, 0.0f));

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    vec3 get_position() const;
    vec3 get_scale() const;
    quat get_rotation() const;
    vec3 get_rotation_euler() const;

    void set_position(const vec3& position);
    void set_scale(const vec3& scale);
    void set_rotation(const quat& rotation);
    void set_rotation_euler(const vec3& rotation);

    mat4 get_model_matrix();
    mat4 get_parent_model_matrix() const;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_position), CEREAL_NVP(m_scale), CEREAL_NVP(m_rotation));
    }

   protected:
    mat4 get_model_matrix_internal() const;

    vec3 m_position;
    vec3 m_scale;
    quat m_rotation;

    bool m_isDirty;
    mat4 m_modelMatrix;
    mat4 m_parentModelMatrix;
};

}  // namespace components
}  // namespace knot
