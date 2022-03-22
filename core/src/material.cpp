#include "knoting/material.h"

namespace knot {
namespace components {

void Material::on_awake() {
    // TODO pass in shader
    m_shader.load_shader("paint", "vs_paint.bin", "fs_paint.bin");
    // end TODO

    // TODO When working update data to tightly packed variables -> 4 floats into 1 vec4
    // clang-format off
    m_uniformHandles[(size_t)UniformHandle::AlbedoColor]        = bgfx::createUniform("m_albedoColor",        bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::TextureTiling]      = bgfx::createUniform("m_textureTiling",      bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::AlbedoScalar]       = bgfx::createUniform("m_albedoScalar",       bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::NormalScalar]       = bgfx::createUniform("m_normalScalar",       bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::MetallicScalar]     = bgfx::createUniform("m_metallicScalar",     bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::RoughnessScalar]    = bgfx::createUniform("m_roughnessScalar",    bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::OcclusionScalar]    = bgfx::createUniform("m_occlusionScalar",    bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::SkyboxScalar]       = bgfx::createUniform("m_skyboxScalar",       bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::CastShadows]        = bgfx::createUniform("m_castShadows",        bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::ReceivesShadows]    = bgfx::createUniform("m_receivesShadows",    bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::AlphaCutoffEnabled] = bgfx::createUniform("m_alphaCutoffEnabled", bgfx::UniformType::Vec4);
    m_uniformHandles[(size_t)UniformHandle::AlphaCutoffAmount]  = bgfx::createUniform("m_alphaCutoffAmount",  bgfx::UniformType::Vec4);

    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Albedo]    = bgfx::createUniform("m_albedo",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Normal]    = bgfx::createUniform("m_normal",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Metallic]  = bgfx::createUniform("m_metallic",  bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Roughness] = bgfx::createUniform("m_roughness", bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Occlusion] = bgfx::createUniform("m_occlusion", bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::RedNormal] = bgfx::createUniform("m_redNormal", bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)UniformSamplerHandle::BlueNormal] = bgfx::createUniform("m_blueNormal", bgfx::UniformType::Sampler);

    m_albedo    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)TextureHandle::Albedo]).lock();
    m_normal    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)TextureHandle::Normal]).lock();

    m_metallic  = std::make_shared<components::Texture>("mask.png");
    m_metallic->on_awake();
    m_metallic->set_pixel(512,512, vec4(1,0,0,1));

    m_roughness = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)TextureHandle::Roughness]).lock();
    m_occlusion = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)TextureHandle::Occlusion]).lock();
    m_redNormal = AssetManager::load_asset<components::Texture>("water_normal.png").lock();
    m_blueNormal = AssetManager::load_asset<components::Texture>("water_normal.png").lock();

    m_textureHandles[(size_t)TextureHandle::Albedo]    = m_albedo->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::Normal]    = m_normal->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::Metallic]  = m_metallic->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::Roughness] = m_roughness->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::Occlusion] = m_occlusion->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::RedNormal] = m_redNormal->get_texture_handle();
    m_textureHandles[(size_t)TextureHandle::BlueNormal] = m_blueNormal->get_texture_handle();

    // clang-format on
}

void Material::on_destroy() {
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::AlbedoColor]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::TextureTiling]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::AlbedoScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::NormalScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::MetallicScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::RoughnessScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::OcclusionScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::SkyboxScalar]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::CastShadows]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::ReceivesShadows]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::AlphaCutoffEnabled]);
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::AlphaCutoffAmount]);

    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Albedo]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Normal]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Metallic]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Roughness]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Occlusion]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::RedNormal]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)UniformSamplerHandle::BlueNormal]);

    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::Albedo]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::Normal]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::Metallic]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::Roughness]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::Occlusion]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::RedNormal]);
    bgfx::destroy(m_textureHandles[(size_t)TextureHandle::BlueNormal]);
}

Material::Material() {
    for (size_t i = 0; i < (size_t)UniformHandle::LAST; ++i) {
        m_uniformHandles[i] = BGFX_INVALID_HANDLE;
    }

    for (size_t i = 0; i < (size_t)UniformSamplerHandle::LAST; ++i) {
        m_uniformSamplerHandle[i] = BGFX_INVALID_HANDLE;
    }

    for (size_t i = 0; i < (size_t)TextureHandle::LAST; ++i) {
        m_textureHandles[i] = BGFX_INVALID_HANDLE;
    }
}

Material::~Material() {}

void Material::set_uniforms() {
    // clang-format off

    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::AlbedoColor],        &m_albedoColor[0]);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::TextureTiling],      &m_textureTiling);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::AlbedoScalar],       &m_albedoScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::NormalScalar],       &m_normalScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::MetallicScalar],     &m_metallicScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::RoughnessScalar],    &m_roughnessScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::OcclusionScalar],    &m_occlusionScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::SkyboxScalar],       &m_skyboxScalar);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::CastShadows],        &m_castShadows);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::ReceivesShadows],    &m_receivesShadows);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::AlphaCutoffEnabled], &m_alphaCutoffEnabled);
    bgfx::setUniform(m_uniformHandles[(size_t)UniformHandle::AlphaCutoffAmount],  &m_alphaCutoffAmount);

    bgfx::setTexture(0, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Albedo],   m_textureHandles[(size_t)TextureHandle::Albedo]);
    bgfx::setTexture(1, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Normal],   m_textureHandles[(size_t)TextureHandle::Normal]);
    bgfx::setTexture(2, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Metallic], m_textureHandles[(size_t)TextureHandle::Metallic]);
    bgfx::setTexture(3, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Roughness],m_textureHandles[(size_t)TextureHandle::Roughness]);
    bgfx::setTexture(4, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::Occlusion],m_textureHandles[(size_t)TextureHandle::Occlusion]);
    bgfx::setTexture(5, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::RedNormal],m_textureHandles[(size_t)TextureHandle::RedNormal]);
    bgfx::setTexture(6, m_uniformSamplerHandle[(size_t)UniformSamplerHandle::BlueNormal],m_textureHandles[(size_t)TextureHandle::BlueNormal]);

    // clang-format off
}

void Material::set_texture_slot_path(const TextureType slot, const std::string& path){
    if ((size_t)slot >= (size_t)TextureType::LAST){
        log::error("invalid material texture : {}", (size_t)slot);
        return;
    }
    m_textureSlotPath[(int)slot] = path;
}

}  // namespace components
}  // namespace knot
