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
    void on_awake() override;
    void on_destroy() override;
    //=For Asset=======
    void generate_default_asset() override;
    //=================

    void generate_solid_color_texture(const vec4& color, const std::string& name);
    void load_texture(const std::string& path);
    void load_texture_2d(const std::string& path, bool usingMipMaps = true, bool usingAnisotropicFiltering = true);
    void load_texture_hdri(const std::string& path);

    bgfx::TextureHandle get_texture_handle() { return m_textureHandle; }

   private:
    bgfx::TextureHandle internal_generate_solid_texture(const vec4& color, const std::string& name);

   private:
    bgfx::TextureHandle m_textureHandle;
    uint16_t width = 0;
    uint16_t height = 0;

    //  PNG 1/2/4/8/16-bit-per-channel
    //  JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
    //  TGA (not sure what subset, if a subset)
    //  BMP non-1bpp, non-RLE
    //  PSD (composited view only, no extra channels, 8/16 bit-per-channel)
    //  HDR (radiance rgbE format)

    static constexpr std::string_view FORMAT_PNG = ".png";
    static constexpr std::string_view FORMAT_JPEG = ".jpeg";
    static constexpr std::string_view FORMAT_JPG = ".jpg";
    static constexpr std::string_view FORMAT_TGA = ".tga";
    static constexpr std::string_view FORMAT_BMP = ".bmp";
    static constexpr std::string_view FORMAT_PSD = ".psd";
    static constexpr std::string_view FORMAT_HDR = ".hdr";

    std::vector<std::string_view> m_supported_texture_2d{FORMAT_PNG, FORMAT_JPG, FORMAT_JPEG,
                                                         FORMAT_TGA, FORMAT_BMP, FORMAT_PSD};
    std::vector<std::string_view> m_supported_skybox{FORMAT_HDR};
};

}  // namespace components
}  // namespace knot
