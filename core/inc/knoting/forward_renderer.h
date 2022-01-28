#pragma once

#include "subsystem.h"

namespace knot {

class ForwardRenderer : public Subsystem {
   public:
    ForwardRenderer();
    ~ForwardRenderer();

    void on_render();
    void on_post_render();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void recreate_framebuffer(uint16_t width, uint16_t height, uint16_t id = 0);
    void clear_framebuffer(uint16_t id = 0);

   private:
    const uint32_t m_clearColor = 0x303030ff;
};

}  // namespace knot
