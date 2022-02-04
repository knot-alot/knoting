
#include <bgfx/bgfx.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>

namespace knot {
namespace components {

enum class UniformHandle {
    AlbedoColor,
    TextureTiling,
    AlbedoScalar,
    NormalScalar,
    MetallicScalar,
    RoughnessScalar,
    OcclusionScalar,
    SkyboxScalar,
    CastShadows,
    ReceivesShadows,
    AlphaCutoffEnabled,
    AlphaCutoffAmount,
    LAST
};

enum class TextureHandle {
    Albedo,
    Normal,
    Metallic,
    Roughness,
    Occlusion,
    LAST
};

class Material {
   public:
    Material();
    ~Material();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

   private:

    std::array<bgfx::UniformHandle,(size_t)UniformHandle::LAST> m_textureHandles;
    std::array<bgfx::TextureHandle,(size_t)TextureHandle::LAST> m_uniformHandles;

   private:
    Texture m_albedo;
    Texture m_normal;
    Texture m_metallic;
    Texture m_roughness;
    Texture m_occlusion;

    ShaderProgram m_shader;

   private:
    float m_albedoScalar = 1.0f;
    float m_normalScalar = 1.0f;
    float m_metallicScalar = 1.0f;
    float m_roughnessScalar = 1.0f;
    float m_occlusionScalar = 1.0f;
    float m_skyboxScalar = 0.2f;

    glm::vec4 m_valueAlbedoColor = glm::vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
    glm::vec2 m_textureTiling = glm::vec2(1);

    bool m_castShadows = true;
    bool m_receivesShadows = true;

    bool m_alphaCutoffEnabled = false;
    float m_alphaCutoffAmount = 0.0f;
};

}  // namespace components
}  // namespace knot
