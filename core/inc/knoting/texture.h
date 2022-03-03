#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <knoting/asset.h>
#include <knoting/asset_manager.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <filesystem>
#include <string>

namespace knot {
namespace components {

class Texture : public Asset {
   public:
    Texture();
    Texture(const std::string& path);
    ~Texture();

    //=For ECS========
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void generate_solid_color_texture(const vec4& color, const std::string& name);
    void load_texture_2d(const std::string& path, bool usingMipMaps = true, bool usingAnisotropicFiltering = true);

    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_assetType), CEREAL_NVP(m_fallbackName), CEREAL_NVP(m_fullPath), CEREAL_NVP(m_assetName),
                CEREAL_NVP(m_width), CEREAL_NVP(m_height));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_assetType), CEREAL_NVP(m_fallbackName), CEREAL_NVP(m_fullPath), CEREAL_NVP(m_assetName),
                CEREAL_NVP(m_width), CEREAL_NVP(m_height));
        m_assetState = AssetState::Idle;
        on_awake();
    }

    bgfx::TextureHandle get_texture_handle() { return m_textureHandle; }

   private:
    bgfx::TextureHandle internal_generate_solid_texture(const vec4& color, const std::string& name);

   private:
    bgfx::TextureHandle m_textureHandle;
    uint16_t m_width = 0;
    uint16_t m_height = 0;
};

}  // namespace components
}  // namespace knot
