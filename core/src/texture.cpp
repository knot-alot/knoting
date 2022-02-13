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
    if (m_assetState == AssetState::IDLE) {
        m_assetState = AssetState::LOADING;
        load_texture_2d(m_fullPath);
    }
}

void Texture::on_destroy() {
    bgfx::destroy(m_textureHandle);
    log::info("removed texture : {}", m_fullPath);
}

// TODO Return fail state for the template so that it can set the idX to the default fallback texture
void Texture::load_texture_2d(const std::string& path, bool usingMipMaps, bool usingAnisotropicFiltering) {
    std::string fullPath = PATH_TEXTURE + path;
    std::filesystem::path fs_path = fullPath;

    if (!exists(fs_path)) {
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

    // clang-format off
    bgfx::TextureHandle textureHandle;

    if (data) {
        textureHandle =
            bgfx::createTexture2D(
            img_size.x,
            img_size.y,
            usingMipMaps,
            numberOfLayers,
            bgfx::TextureFormat::RGBA8,
            textureFlags,
            bgfx::copy(data, img_size.x * img_size.y * channels));

    m_assetState = AssetState::FINISHED;
    } else {
        textureHandle = generate_solid_texture(vec4(1,1,1,1), "fallback");

        // TODO Consider generating the fallback texture before loading any textures and then setting the handle to it by default in the load_asset template class
        log::info("generated fallback texture");
        m_assetState = AssetState::FAILED;

    }

    // clang-format on

    if (!bgfx::isValid(textureHandle)) {
        log::error("Error loading texture " + path);
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    m_textureHandle = textureHandle;
}

bgfx::TextureHandle Texture::generate_solid_texture(const vec4& color, const std::string& name) {

    m_fullPath = name;

    const int x{2};
    const int y{2};
    const int rgba{4};
    const int layers{1};
    const bool mips{false};

    const uint32_t flags =
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;

    uint8_t r = (uint8_t)(255);
    uint8_t g = (uint8_t)(255);
    uint8_t b = (uint8_t)(255);
    uint8_t a = (uint8_t)(255);

    unsigned char texData[x * y * rgba] = {255, 255, 255, 255, 255, 255, 255, 255,
                                           255, 255, 255, 255, 255, 255, 255, 255};

    unsigned char* imageData = (unsigned char*)texData;

    return bgfx::createTexture2D(x, y, mips, layers, bgfx::TextureFormat::RGBA8, flags,
                                 bgfx::copy(imageData, x * y * rgba));

    // TODO Consider generating the fallback texture before loading any textures and then setting the handle to it by
    // default in the load_asset template class
    log::info("generated solid texture");
}
void Texture::generate_default_asset() {
    m_assetState = AssetState::LOADING;
    bgfx::TextureHandle textureHandle = generate_solid_texture(vec4(1, 0, 1, 1), "fallbackTexture");

    if (!bgfx::isValid(textureHandle)) {
        log::error("fallbackTexture failed to be created");
        m_textureHandle = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::FAILED;
        return;
    }

    m_textureHandle = textureHandle;
    m_assetState = AssetState::FINISHED;
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
