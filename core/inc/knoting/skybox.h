#pragma once
#include <knoting/asset_manager.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>
#include <knoting/types.h>
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

    void set_uniforms();
    bgfx::ProgramHandle get_program() { return m_shader.get_program(); };

   private:
    std::array<bgfx::UniformHandle, (size_t)SkyBoxUniformSamplerHandle::LAST> m_uniformSamplerHandle;
    std::array<bgfx::TextureHandle, (size_t)SkyBoxTextureHandle::LAST> m_textureHandles;

   private:
    std::shared_ptr<Texture> m_albedo;
    std::shared_ptr<Texture> m_normal;
    std::shared_ptr<Texture> m_metallic;

    std::string m_textureSlotPath[3] = {"", "", ""};

    ShaderProgram m_shader;
};
}  // namespace components
}  // namespace knot
