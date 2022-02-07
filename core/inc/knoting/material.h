#pragma once

#include <bgfx/bgfx.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>

namespace knot {
namespace components {
// clang-format off

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

enum class UniformSamplerHandle {
    Albedo,
    Normal,
    Metallic,
    Roughness,
    Occlusion,
    LAST
};

// clang-format on

class Material {
   public:
    Material();
    ~Material();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void set_uniforms();
    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

   private:
    std::array<bgfx::UniformHandle, (size_t)UniformHandle::LAST> m_uniformHandles;
    std::array<bgfx::UniformHandle, (size_t)UniformSamplerHandle::LAST> m_uniformSamplerHandle;
    std::array<bgfx::TextureHandle, (size_t)TextureHandle::LAST> m_textureHandles;

   private:
    Texture m_albedo;
    Texture m_normal;
    Texture m_metallic;
    Texture m_roughness;
    Texture m_occlusion;

    ShaderProgram m_shader;

   private:
    // TODO consider mapping uniforms to string
    glm::vec4 m_albedoColor = glm::vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
    glm::vec2 m_textureTiling = glm::vec2(1);

    float m_albedoScalar = 1.0f;
    float m_normalScalar = 1.0f;
    float m_metallicScalar = 1.0f;
    float m_roughnessScalar = 1.0f;
    float m_occlusionScalar = 1.0f;
    float m_skyboxScalar = 0.2f;

    bool m_castShadows = true;
    bool m_receivesShadows = true;

    bool m_alphaCutoffEnabled = false;
    float m_alphaCutoffAmount = 0.0f;
    // end TODO
};

}  // namespace components
}  // namespace knot
