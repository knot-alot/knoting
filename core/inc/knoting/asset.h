#pragma once

#include <knoting/types.h>
#include <string>

static constexpr std::string_view PATH_TEXTURE = "textures/";
static constexpr std::string_view PATH_MODELS = "misc/";
static constexpr std::string_view PATH_SHADER = "shaders/";

static constexpr std::string_view fallbackTextureName = "fallbackTexture";
static constexpr std::string_view fallbackMeshName = "fallbackMesh";
static constexpr std::string_view fallbackShaderName = "fallbackShader";
static constexpr std::string_view fallbackCubeMapName = "fallbackCubeMap";

namespace knot {
using namespace asset;

class Asset {
   public:
    Asset(AssetType assetType, const std::string& path);
    virtual void on_awake() = 0;
    virtual void on_destroy() = 0;
    virtual void generate_default_asset() = 0;

    AssetState get_asset_state() { return m_assetState; };
    std::string get_fallback_name() { return m_fallbackName; };

   private:
    void set_fallback_name(AssetType type);

   protected:
    AssetType m_assetType = AssetType::Unknown;
    AssetState m_assetState = AssetState::Idle;

    std::string m_fallbackName;

    std::string m_fullPath;
    std::string m_assetName;
    std::string m_assetDirectory;
};

}  // namespace knot
