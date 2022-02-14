#pragma once

#include <knoting/types.h>
#include <string>

static constexpr char PATH_SHADER[] = "../res/shaders/";
static constexpr char PATH_TEXTURE[] = "../res/textures/";
static constexpr char PATH_MODELS[] = "../res/models/";

namespace knot {
using namespace asset;

class Asset {
   public:
    Asset(AssetType assetType, const std::string& path, const std::string& fallbackName);
    virtual void on_awake() = 0;
    virtual void on_destroy() = 0;
    virtual void generate_default_asset() = 0;

    AssetState get_asset_state() { return m_assetState; };
    std::string get_fallback_name() { return m_fallbackName; };

   protected:
    AssetType m_assetType = AssetType::Unknown;
    AssetState m_assetState = AssetState::Idle;

    std::string m_fallbackName = "SET_IN_DERIVED_IMPLEMENTATION";

    std::string m_fullPath;
    std::string m_assetName;
    std::string m_assetDirectory;
};

}  // namespace knot
