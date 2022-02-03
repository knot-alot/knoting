
#include <knoting/log.h>
#include <knoting/texture.h>
#include <fstream>

namespace knot {
Texture::~Texture() {
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
}  // namespace knot