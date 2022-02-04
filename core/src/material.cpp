#include <knoting/material.h>
namespace knot {
namespace components {

void Material::on_awake() {
//    m_uniformHandles[(size_t)UniformHandle::AlbedoColor] = bgfx::createUniform("u_albedoColor", bgfx::UniformType::);
}

void Material::on_destroy() {
    bgfx::destroy(m_uniformHandles[(size_t)UniformHandle::AlbedoColor]);
}

Material::Material() {
    std::fill(m_uniformHandles.begin(), m_uniformHandles.end(), bgfx::kInvalidHandle);
    std::fill(m_textureHandles.begin(), m_textureHandles.end(), bgfx::kInvalidHandle);
}

}  // namespace components
}  // namespace knot