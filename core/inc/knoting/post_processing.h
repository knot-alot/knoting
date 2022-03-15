#include <bgfx/bgfx.h>
#include <knoting/components.h>
#include <knoting/types.h>
#include <memory>

namespace knot {
namespace components {

class PostProcessing {
   public:
    PostProcessing();
    ~PostProcessing();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void set_geometry_framebuffer(const bgfx::FrameBufferHandle& fbh);
    void set_gui_framebuffer(const bgfx::FrameBufferHandle& fbh);

    bgfx::TextureHandle get_color_geometry_render_texture();
    bgfx::TextureHandle get_color_gui_render_texture();

    uint16_t get_geometry_id();
    uint16_t get_gui_id();

   private:
    // MAX PS4 Attachments 8
    bgfx::FrameBufferHandle m_geometryPass;
    bgfx::FrameBufferHandle m_guiPass;
};
}  // namespace components
}  // namespace knot
