#include <knoting/log.h>
#include <knoting/texture.h>
#include <stb_image.h>
#include <fstream>

namespace knot {
namespace components {

Texture::Texture() : Asset{AssetType::Texture, ""} {}
Texture::Texture(const std::string& path) : Asset{AssetType::Texture, path} {}
Texture::~Texture() {}

void Texture::on_awake() {
    if (m_assetState != AssetState::Finished) {
        m_assetState = AssetState::Loading;
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
        m_assetState = AssetState::Failed;
        return;
    }

    uint32_t textureFlags{0};
    // TODO check if bgfx has anisotropic is supported flag

    if (usingAnisotropicFiltering) {
        textureFlags =
            BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
    } else {
        textureFlags = BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
    }

    // TODO stbi does not support mips find a way to get mips working
    usingMipMaps = false;
    if (usingMipMaps) {
        textureFlags += BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    }

    // clang-format off
    bgfx::TextureHandle textureHandle;

    textureHandle =
        bgfx::createTexture2D(
        img_size.x,
        img_size.y,
        usingMipMaps,
        numberOfLayers,
        bgfx::TextureFormat::RGBA8,
        textureFlags,
        bgfx::copy(data, img_size.x * img_size.y * channels));

    m_assetState = AssetState::Finished;

    // clang-format on

    if (!bgfx::isValid(textureHandle)) {
        log::error("Error loading texture " + path);
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    m_textureHandle = textureHandle;
}

bgfx::TextureHandle Texture::internal_generate_solid_texture(const vec4& color, const std::string& name) {
    // clang-format off

    m_fullPath = name;

    constexpr int x = 2;
    constexpr int y = 2;
    constexpr int rgba = 4;
    constexpr int layers = 1;
    constexpr bool mips = false;

    constexpr uint32_t flags =
        BGFX_SAMPLER_W_CLAMP |
        BGFX_SAMPLER_W_CLAMP |
        BGFX_SAMPLER_MIN_POINT |
        BGFX_SAMPLER_MAG_POINT;

    uint8_t r = (uint8_t)(color.r * 255);
    uint8_t g = (uint8_t)(color.g * 255);
    uint8_t b = (uint8_t)(color.b * 255);
    uint8_t a = (uint8_t)(color.a * 255);

    unsigned char texData[x * y * rgba] = {
    r, g, b, a,
    r, g, b, a,
    r, g, b, a,
    r, g, b, a
    };

    unsigned char* imageData = (unsigned char*)texData;

    return bgfx::createTexture2D(
        x,
        y,
        mips,
        layers,
        bgfx::TextureFormat::RGBA8,
        flags,
        bgfx::copy(imageData, x * y * rgba)
    );
    // clang-format on
}
void Texture::generate_default_asset() {
    m_assetState = AssetState::Loading;
    bgfx::TextureHandle textureHandle = internal_generate_solid_texture(vec4(1, 0, 1, 1), "fallbackTexture");

    if (!bgfx::isValid(m_textureHandle)) {
        log::error("fallbackTexture failed to be created");
        m_textureHandle = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
        return;
    }

    m_textureHandle = textureHandle;
    m_assetState = AssetState::Finished;
}

void Texture::generate_solid_color_texture(const vec4& color, const std::string& name) {
    m_assetState = AssetState::Loading;
    bgfx::TextureHandle textureHandle = internal_generate_solid_texture(color, name);

    if (!bgfx::isValid(m_textureHandle)) {
        log::error("solid texture Name : {} - of color : [{},{},{},{}] failed to be created", name, color.r, color.g,
                   color.b, color.a);
        m_textureHandle = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
        return;
    }

    m_textureHandle = textureHandle;
    m_assetState = AssetState::Finished;
}
}  // namespace components
}  // namespace knot
