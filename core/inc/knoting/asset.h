#pragma once

#include <knoting/types.h>
#include <string>

static constexpr char PATH_SHADER[] = "../res/shaders/";
static constexpr char PATH_TEXTURE[] = "../res/textures/";
static constexpr char PATH_MODELS[] = "../res/models/";

namespace knot {
using namespace asset;

enum class AssetState { IDLE, LOADING, FINISHED, FAILED };

class Asset {
   public:
    Asset(AssetType assetType, const std::string& path);
    virtual void on_awake() = 0;
    virtual void on_destroy() = 0;
   protected:
    AssetType m_assetType = AssetType::UNKNOWN;

    std::string m_fullPath;
    std::string m_assetName;
    std::string m_assetDirectory;
};

}  // namespace knot
