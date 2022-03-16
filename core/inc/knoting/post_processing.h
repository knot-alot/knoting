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

    void set_color_render_texture(const bgfx::TextureHandle& tbh);
    void set_gui_render_texture(const bgfx::TextureHandle& tbh);

    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

   private:
    // MAX PS4 Attachments 8
    bgfx::FrameBufferHandle m_geometryPass;
    bgfx::FrameBufferHandle m_guiPass;

    bgfx::TextureHandle m_geoTBH;
    bgfx::TextureHandle m_guiTBH;

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
