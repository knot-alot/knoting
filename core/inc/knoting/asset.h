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
    Asset(AssetType assetType, const std::string& path);
    virtual void on_awake() = 0;
    virtual void on_destroy() = 0;

    AssetState get_asset_state(){return m_assetState;};

    std::string fallback_name(){

    }

   protected:
    AssetType m_assetType = AssetType::UNKNOWN;
    AssetState m_assetState = AssetState::IDLE;


    std::string m_fullPath;
    std::string m_assetName;
    std::string m_assetDirectory;
};

}  // namespace knot
