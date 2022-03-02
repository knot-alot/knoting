#pragma once
#include <knoting/asset_manager.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>
#include <memory>

namespace knot {
namespace components {
// clang-format off

class SkyBox {
   public:
    SkyBox();
    ~SkyBox();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void set_texture_slot_path(TextureType slot, const std::string& path);

    void set_uniforms();
    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

   private:
    std::array<bgfx::UniformHandle, (size_t)UniformHandle::LAST> m_uniformHandles;
    std::array<bgfx::UniformHandle, (size_t)UniformSamplerHandle::LAST> m_uniformSamplerHandle;
    std::array<bgfx::TextureHandle, (size_t)TextureHandle::LAST> m_textureHandles;

   private:
    std::shared_ptr<Texture> m_albedo;
    std::shared_ptr<Texture> m_normal;
    std::shared_ptr<Texture> m_metallic;
    std::shared_ptr<Texture> m_roughness;
    std::shared_ptr<Texture> m_occlusion;

    std::string m_textureSlotPath[5] = {"", "", "", "", ""};

    ShaderProgram m_shader;

   private:
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
};
}  // namespace components
}  // namespace knot
