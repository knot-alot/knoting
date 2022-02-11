#include <knoting/log.h>
#include <knoting/texture.h>
#include <stb_image.h>
#include <fstream>

namespace knot {
namespace components {

Texture::Texture() : Asset{AssetType::TEXTURE, ""} {}
Texture::Texture(const std::string& path) : Asset{AssetType::TEXTURE, path} {}
Texture::~Texture() {}

void Texture::on_awake() {
    load_texture_2d(m_fullPath);
}

void Texture::on_destroy() {
    bgfx::destroy(m_textureHandle);
}

void Texture::load_texture_2d(const std::string& path, bool usingMipMaps, bool usingAnisotropicFiltering) {
    std::string fullPath = PATH_TEXTURE + path;
    std::filesystem::path fs_path = fullPath;

    if (exists(fs_path)) {
        log::debug(fullPath + " - Exists");
    } else {
        log::error(fullPath + " - does not Exist");
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    // load image with stb_image
    glm::ivec2 img_size;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(fullPath.c_str(), &img_size.x, &img_size.y, &channels, 0);
    int numberOfLayers = 1;

    if (!data) {
        log::error("Failed to load image: " + fullPath);
    }

    uint32_t textureFlags{0};
    // TODO check if bgfx has anisotropic is supported flag

    if (usingAnisotropicFiltering) {
        textureFlags =
            BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
    } else {
        textureFlags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
    }

    // TODO stbi does not support mips find a way to get mips working
    usingMipMaps = false;
    if (usingMipMaps) {
        textureFlags += BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    }

    bgfx::TextureHandle textureHandle =
        bgfx::createTexture2D(img_size.x, img_size.y, usingMipMaps, numberOfLayers, bgfx::TextureFormat::RGBA8,
                              textureFlags, bgfx::copy(data, img_size.x * img_size.y * channels));

    if (!bgfx::isValid(textureHandle)) {
        log::error("Error loading texture " + path);
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    m_textureHandle = textureHandle;
}

// TODO impl in knot -> file from Proc_GL by @beardyKing
// void Texture2D::GenerateFallbackTexture() {
//    glGenTextures(1, &m_texture);
//
//    GLenum format;
//    format = GL_RGBA;
//    GLubyte texData[] = { 255, 255, 255, 255 };			// Generate white single pixel texture with full
//    alpha unsigned char* imageData = (unsigned char*)texData;
//
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, format, 1, 1, 0, format, GL_UNSIGNED_BYTE, imageData);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    std::cout << "generated fallback texture: " << std::endl;
//}

}  // namespace components
}  // namespace knot
