#pragma once

#include <bgfx/bgfx.h>
#include <knoting/asset_manager.h>
#include <knoting/component.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>

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
    RedNormal,
    BlueNormal,
    LAST
};

enum class UniformSamplerHandle {
    Albedo,
    Normal,
    Metallic,
    Roughness,
    Occlusion,
    RedNormal,
    BlueNormal,
    LAST
};

class Material : public Component<Material> {
   public:
    Material();
    ~Material();

    void on_awake() override;
    void on_destroy() override;

    void set_texture_slot_path(const TextureType slot, const std::string& path);
    void set_albedo_mix_color(const vec4& color){m_albedoColor = color;};
    void set_uniforms();

    void set_mask_data(const vec4 data[100]);

    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

    void set_texture_scale(vec2 textureScale);

    template <class Archive>
    void save(Archive &archive) const{
        archive(CEREAL_NVP(m_textureSlotPath), CEREAL_NVP(m_shader),CEREAL_NVP(m_albedoColor),CEREAL_NVP(m_textureTiling),
        CEREAL_NVP(m_albedoScalar),CEREAL_NVP(m_normalScalar),CEREAL_NVP(m_metallicScalar),CEREAL_NVP(m_roughnessScalar),CEREAL_NVP(m_occlusionScalar)
        ,CEREAL_NVP(m_skyboxScalar),CEREAL_NVP(m_castShadows),CEREAL_NVP(m_receivesShadows),CEREAL_NVP(m_alphaCutoffEnabled),CEREAL_NVP(m_alphaCutoffAmount));
    }

    template <class Archive>
    void load(Archive &archive){
        archive(CEREAL_NVP(m_textureSlotPath), CEREAL_NVP(m_albedoColor),CEREAL_NVP(m_textureTiling),
        CEREAL_NVP(m_albedoScalar),CEREAL_NVP(m_normalScalar),CEREAL_NVP(m_metallicScalar),CEREAL_NVP(m_roughnessScalar),CEREAL_NVP(m_occlusionScalar)
        ,CEREAL_NVP(m_skyboxScalar),CEREAL_NVP(m_castShadows),CEREAL_NVP(m_receivesShadows),CEREAL_NVP(m_alphaCutoffEnabled),CEREAL_NVP(m_alphaCutoffAmount));
        on_awake();
    }

   private:
    std::array<bgfx::UniformHandle, (size_t)UniformHandle::LAST> m_uniformHandles;
    std::array<bgfx::UniformHandle, (size_t)UniformSamplerHandle::LAST> m_uniformSamplerHandle;
    std::array<bgfx::TextureHandle, (size_t)TextureHandle::LAST> m_textureHandles;

    bgfx::UniformHandle m_paintData0;
    bgfx::UniformHandle m_paintData1;
    bgfx::UniformHandle m_paintData2;
    bgfx::UniformHandle m_paintData3;

   private:
    std::shared_ptr<Texture> m_albedo;
    std::shared_ptr<Texture> m_normal;
    std::shared_ptr<Texture> m_metallic;
    std::shared_ptr<Texture> m_roughness;
    std::shared_ptr<Texture> m_occlusion;
    std::shared_ptr<Texture> m_redNormal;
    std::shared_ptr<Texture> m_blueNormal;

    std::string m_textureSlotPath[5] = {"", "", "", "", ""};

    ShaderProgram m_shader;

   private:
    glm::vec4 m_albedoColor = glm::vec4(1,1,1,1);
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
