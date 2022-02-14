#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <knoting/asset.h>
#include <knoting/types.h>
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
    void on_awake();
    void on_destroy();
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void generate_solid_color_texture(const vec4& color, const std::string& name);
    void load_texture_2d(const std::string& path, bool usingMipMaps = true, bool usingAnisotropicFiltering = true);

    bgfx::TextureHandle get_texture_handle() { return m_textureHandle; }

   private:
    bgfx::TextureHandle internal_generate_solid_texture(const vec4& color, const std::string& name);

   private:
    bgfx::TextureHandle m_textureHandle;
    uint16_t width = 0;
    uint16_t height = 0;
};

}  // namespace components
}  // namespace knot
