#include "knoting/health.h"

namespace knot::components {
void Health::on_awake() {
    m_max_health = 1000;
}
void Health::on_destroy() {}

}  // namespace knot::components