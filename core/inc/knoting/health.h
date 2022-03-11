#pragma once
#include <knoting/components.h>

namespace knot::components {
class Health {
   public:
    Health() = default;
    ~Health() = default;

    void on_awake();
    void on_destroy();

    int get_health() const { return m_health; };
    void set_health(int hp) { m_health = hp; }

    int get_max_health() const { return m_max_health; };
    void set_max_health(int hp) { m_max_health = hp; }

   private:
    int m_health;
    int m_max_health;
};

}  // namespace knot::components