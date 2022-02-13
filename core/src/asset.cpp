#include <knoting/asset.h>

#include <knoting/asset_loaders/image_loader.h>
#include <knoting/asset_loaders/model_loader.h>
#include <filesystem>

#include <knoting/log.h>

namespace knot {

Asset::Asset(AssetType assetType, const std::string& path, const std::string& fallbackName) : m_assetType(assetType) {
    m_fallbackName = fallbackName;
    m_fullPath = path;
    m_assetName = std::filesystem::path(m_fullPath).filename().string();
    m_assetDirectory = std::filesystem::path(m_fullPath).parent_path().string();
}
}  // namespace knot