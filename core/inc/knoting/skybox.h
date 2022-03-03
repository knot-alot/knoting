#pragma once
#include <knoting/asset_manager.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <memory>

namespace knot {
namespace components {

// clang-format off

enum class SkyBoxUniformSamplerHandle { SkyBox, Irradiance, Radiance, LAST };
enum class SkyBoxTextureHandle { SkyBox, Irradiance, Radiance, LAST };

// clang-format on

class SkyBox {
   public:
    SkyBox();
    ~SkyBox();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void set_texture_slot_path(SkyBoxTextureType slot, const std::string& path);
    void screenSpaceQuad(float _textureWidth,
                         float _textureHeight,
                         bool _originBottomLeft = false,
                         float _width = 1.0f,
                         float _height = 1.0f);

    void set_uniforms();
    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

    void set_uniform_exposure(float exposure) { m_exposure = exposure; };
    float get_uniform_exposure() { return m_exposure; };

    void set_uniform_background_type(SkyBoxTextureType type) { m_backgroundType = type; };
    SkyBoxTextureType get_uniform_background_type() { return m_backgroundType; };

   private:
    std::array<bgfx::UniformHandle, (size_t)SkyBoxUniformSamplerHandle::LAST> m_uniformSamplerHandle;
    std::array<bgfx::TextureHandle, (size_t)SkyBoxTextureHandle::LAST> m_textureHandles;

   private:
    std::shared_ptr<Texture> m_albedo;
    std::shared_ptr<Texture> m_normal;
    std::shared_ptr<Texture> m_metallic;

    std::string m_textureSlotPath[3] = {"", "", ""};
    bgfx::UniformHandle m_uniformSkyboxData;
    vec4 m_skyboxData;

    float m_exposure = 1.0f;
    SkyBoxTextureType m_backgroundType = SkyBoxTextureType::Irradiance;

    ShaderProgram m_shader;

   public:
    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_textureSlotPath), CEREAL_NVP(m_shader), CEREAL_NVP(m_exposure),
                CEREAL_NVP(m_backgroundType));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_textureSlotPath), CEREAL_NVP(m_shader), CEREAL_NVP(m_exposure),
                CEREAL_NVP(m_backgroundType));
        on_awake();
    }
};
}  // namespace components
}  // namespace knot
