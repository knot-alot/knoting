#include "knoting/skybox.h"

namespace knot {
namespace components {
void SkyBox::on_awake() {
    m_shader.load_shader("skyboxport", "vs_skybox.bin", "fs_skybox.bin");

    // clang-format off

    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox]          = bgfx::createUniform("s_texCube",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance]      = bgfx::createUniform("s_texCubeIrr",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance]        = bgfx::createUniform("m_metallic",  bgfx::UniformType::Sampler);

    m_uniformSkyboxData = bgfx::createUniform("m_skyboxData",    bgfx::UniformType::Vec4);

    m_albedo    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::SkyBox]).lock();
    m_normal    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::Irradiance]).lock();
    m_metallic  = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::Radiance]).lock();

    m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]           = m_albedo->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]       = m_normal->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]         = m_metallic->get_texture_handle();

    // clang-format on
}

void SkyBox::on_destroy() {
    bgfx::destroy(m_uniformSkyboxData);

    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance]);

    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]);
}

SkyBox::SkyBox() {
    m_uniformSkyboxData = BGFX_INVALID_HANDLE;

    for (size_t i = 0; i < (size_t)SkyBoxUniformSamplerHandle::LAST; ++i) {
        m_uniformSamplerHandle[i] = BGFX_INVALID_HANDLE;
    }

    for (size_t i = 0; i < (size_t)SkyBoxTextureHandle::LAST; ++i) {
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

    bgfx::setTexture(0, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]);
    bgfx::setTexture(1, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]);
    bgfx::setTexture(2, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]);
}

void SkyBox::set_texture_slot_path(const SkyBoxTextureType slot, const std::string& path) {
    switch (slot) {
        case SkyBoxTextureType::Albedo:
            break;
        case SkyBoxTextureType::Irradiance:
            break;
        case SkyBoxTextureType::Radiance:
            break;
        default:
            return;
    }
    m_textureSlotPath[(int)slot] = path;
}

}  // namespace components
}  // namespace knot
