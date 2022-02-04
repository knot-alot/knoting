#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <knoting/types.h>
#include <filesystem>
#include <string>

namespace knot {
namespace components {

class Texture {
   public:
    Texture();
    ~Texture();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    void load_texture_2d(const std::string& path, bool usingMipMaps = true, bool usingAnisotropicFiltering = true);

    bgfx::TextureHandle get_texture_handle() { return m_textureHandle; }

   private:
    bgfx::TextureHandle m_textureHandle;

    uint16_t width = 0;
    uint16_t height = 0;
    static constexpr char PATH_TEXTURE[] = "../res/textures/";
};

}  // namespace components
}  // namespace knot