#include "knoting/skybox.h"

namespace knot {
namespace components {
void SkyBox::on_awake() {
    m_shader.load_shader("skyboxport", "vs_skybox.bin", "fs_skybox.bin");

    // clang-format off

    m_uniformSamplerHandle[(size_t)SkyBoxTextureType::SkyBox]          = bgfx::createUniform("s_texCube",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Irradiance]      = bgfx::createUniform("s_texCubeIrr",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Radiance]        = bgfx::createUniform("m_metallic",  bgfx::UniformType::Sampler);

    m_uniformSkyboxData = bgfx::createUniform("m_skyboxData",    bgfx::UniformType::Vec4);

    m_albedo    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureType::SkyBox]).lock();
    m_normal    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureType::Irradiance]).lock();
    m_metallic  = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureType::Radiance]).lock();

    m_textureHandles[(size_t)SkyBoxTextureType::SkyBox]           = m_albedo->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureType::Irradiance]       = m_normal->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureType::Radiance]         = m_metallic->get_texture_handle();

    // clang-format on
}

void SkyBox::on_destroy() {
    bgfx::destroy(m_uniformSkyboxData);

    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxTextureType::SkyBox]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Irradiance]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Radiance]);

    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureType::SkyBox]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureType::Irradiance]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureType::Radiance]);
}

SkyBox::SkyBox() {
    m_uniformSkyboxData = BGFX_INVALID_HANDLE;

    for (size_t i = 0; i < (size_t)SkyBoxTextureType::LAST; ++i) {
        m_uniformSamplerHandle[i] = BGFX_INVALID_HANDLE;
    }

    for (size_t i = 0; i < (size_t)SkyBoxTextureType::LAST; ++i) {
        m_textureHandles[i] = BGFX_INVALID_HANDLE;
    }
}

SkyBox::~SkyBox() {}
void SkyBox::set_uniforms() {
    m_skyboxData.x = m_exposure;
    m_skyboxData.y = (int)m_backgroundType;
    m_skyboxData.z = 0.0f;
    m_skyboxData.w = 0.0f;

    bgfx::setUniform(m_uniformSkyboxData, &m_skyboxData);

    bgfx::setTexture(0, m_uniformSamplerHandle[(size_t)SkyBoxTextureType::SkyBox],
                     m_textureHandles[(size_t)SkyBoxTextureType::SkyBox]);
    bgfx::setTexture(1, m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Irradiance],
                     m_textureHandles[(size_t)SkyBoxTextureType::Irradiance]);
    bgfx::setTexture(2, m_uniformSamplerHandle[(size_t)SkyBoxTextureType::Radiance],
                     m_textureHandles[(size_t)SkyBoxTextureType::Radiance]);
}

void SkyBox::set_texture_slot_path(const SkyBoxTextureType slot, const std::string& path) {
    if ((size_t)slot >= (size_t)SkyBoxTextureType::LAST) {
        log::error("invalid skybox texture : {}", (size_t)slot);
        return;
    }
    m_textureSlotPath[(int)slot] = path;
}

}  // namespace components
}  // namespace knot
