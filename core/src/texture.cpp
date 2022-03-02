#include <knoting/asset_manager.h>
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
        load_texture(m_fullPath);
    }
}

void Texture::on_destroy() {
    bgfx::destroy(m_textureHandle);
    log::info("removed texture : {}", m_fullPath);
}

void Texture::load_texture(const std::string& path) {
    auto ext = std::filesystem::path(path).extension().string();

    if (std::count(m_supported_texture_2d.begin(), m_supported_texture_2d.end(), ext)) {
        load_texture_2d(path);
        log::info(ext);
    } else if (std::count(m_supported_skybox.begin(), m_supported_skybox.end(), ext)) {
        log::info("loading HDR texture");
        m_assetState = AssetState::Failed;
        // TODO .hdr files using stbi for loading incorrectly
        load_texture_hdri(path);
    } else {
        log::error("{} on file {} is not a supported texture format", ext, path);
        m_assetState = AssetState::Failed;
    }
}

void Texture::load_texture_hdri(const std::string& path) {
    std::filesystem::path fsPath = AssetManager::get_resources_path().append(PATH_TEXTURE).append(path);

    if (!exists(fsPath)) {
        log::error("{} - does not Exist", fsPath.string());
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    // load image with stb_image
    glm::ivec2 img_size;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(fsPath.string().c_str(), &img_size.x, &img_size.y, &channels, 0);
    int numberOfLayers = 1;

    if (!data) {
        log::error("Failed to load image: {}", fsPath.string());
        m_assetState = AssetState::Failed;
        return;
    }

    uint32_t textureFlags{0};
    textureFlags = BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_W_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;

    // clang-format off
    bgfx::TextureHandle textureHandle;

    textureHandle =
        bgfx::createTexture2D(
        img_size.x,
        img_size.y,
        false,
        numberOfLayers,
        bgfx::TextureFormat::RGBA8,
        textureFlags,
        bgfx::copy(data, img_size.x * img_size.y * channels));

    // clang-format on

    if (!bgfx::isValid(textureHandle)) {
        log::error("Failed to load .hdr only 8/16 bit files supported : {} , ", fsPath.string());
        m_textureHandle = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    m_assetState = AssetState::Finished;
    m_textureHandle = textureHandle;
}

// TODO Return fail state for the template so that it can set the idX to the default fallback texture
void Texture::load_texture_2d(const std::string& path, bool usingMipMaps, bool usingAnisotropicFiltering) {
    std::filesystem::path fsPath = AssetManager::get_resources_path().append(PATH_TEXTURE).append(path);

    if (!exists(fsPath)) {
        log::error("{} - does not Exist", fsPath.string());
        m_textureHandle = BGFX_INVALID_HANDLE;
    }

    // load image with stb_image
    glm::ivec2 img_size;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(fsPath.string().c_str(), &img_size.x, &img_size.y, &channels, 0);
    int numberOfLayers = 1;

    if (!data) {
        log::error("Failed to load image: {}", fsPath.string());
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

    // clang-format on

    if (!bgfx::isValid(textureHandle)) {
        log::error("Error loading texture " + path);
        m_textureHandle = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_image_free(data);
    m_assetState = AssetState::Finished;
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
