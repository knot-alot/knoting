
#include <fstream>
#include <knoting/log.h>
#include <knoting/texture.h>


namespace knot {

Texture::Texture() {}

Texture::~Texture() {}

void Texture::load_texture(const std::string& localTexturePath) {

//    std::string fullPath = PATH_TEXTURE + localTexturePath;
//    std::filesystem::path fs_path = std::filesystem::path(fullPath);
//    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
//
//    uint16_t size;
//    char* raw_image;
//
//    if (exists(fs_path)) {
//        log::debug("file loaded at : " + fullPath);
//
//    } else {
//        log::error("file : " + fullPath + " does not exist");
//    }
}
}  // namespace knot