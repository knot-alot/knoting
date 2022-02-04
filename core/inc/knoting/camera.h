#pragma once

#include <knoting/types.h>

namespace knot{
namespace components{
class EditorCamera {
   public:
    EditorCamera();
    ~EditorCamera();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

   private:

    glm::vec3 m_lookTarget;

    float m_fov = 60.0f;

    float m_zNear = 0.1f;
    float m_zFar = 800.0f;

    float m_moveSpeed = 5;
    float m_moveSpeedMultiplier = 4;
};

}
}
