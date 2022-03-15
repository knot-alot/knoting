#include <bgfx/bgfx.h>
#include <knoting/components.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
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

    void set_uniforms();

    void set_geometry_framebuffer(const bgfx::FrameBufferHandle& fbh);
    void set_gui_framebuffer(const bgfx::FrameBufferHandle& fbh);

    bgfx::TextureHandle get_color_geometry_render_texture();
    bgfx::TextureHandle get_color_gui_render_texture();

    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

    uint16_t get_geometry_id();
    uint16_t get_gui_id();

   private:
    // MAX PS4 Attachments 8
    bgfx::FrameBufferHandle m_geometryPass;
    bgfx::FrameBufferHandle m_guiPass;

    ShaderProgram m_shader;
    bgfx::UniformHandle m_uniformGeometry;
    bgfx::UniformHandle m_uniformGui;

    bool m_depthOfFieldEnabled = false;

   public:
    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_depthOfFieldEnabled));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_depthOfFieldEnabled));
    }
};
}  // namespace components
}  // namespace knot
