#pragma once

#include <string>
#include <filesystem>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>

namespace knot {
class Texture {
   public:
    Texture();
    ~Texture();

    void load_texture(const std::string& localTexturePath);
   private:

   private:
    bgfx::TextureHandle m_textureHandle;


    uint16_t width = 0;
    uint16_t height = 0;

};

}  // namespace knot
