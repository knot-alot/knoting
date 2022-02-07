#include <knoting/camera.h>

namespace knot {
namespace components {

EditorCamera::EditorCamera() {}
EditorCamera::~EditorCamera() {}

void EditorCamera::on_awake() {}
void EditorCamera::on_destroy() {}

vec3 EditorCamera::get_up() {
    constexpr vec3 WORLD_UP = vec3(0,1,0);
    return WORLD_UP;
}

}  // namespace components
}  // namespace knot
