#include <knoting/camera.h>

namespace knot {
namespace components {

EditorCamera::EditorCamera() {}
EditorCamera::~EditorCamera() {
    on_destroy();  // fallback
}

void EditorCamera::on_awake() {
    s_activeCamera = *this;
}

void EditorCamera::on_destroy() {
    if (!s_activeCamera)
        return;
    if (&s_activeCamera.value().get() != this)
        return;

    s_activeCamera = std::nullopt;
}

vec3 EditorCamera::get_up() {
    constexpr vec3 WORLD_UP = vec3(0, 1, 0);
    return WORLD_UP;
}

}  // namespace components
}  // namespace knot
