#include <knoting/asset.h>

#include <knoting/asset_loaders/image_loader.h>
#include <knoting/asset_loaders/model_loader.h>
#include <filesystem>

#include <knoting/log.h>

namespace knot {

Asset::Asset(AssetType assetType, const std::string& path) : m_assetType(assetType) {
    set_fallback_name(assetType);
    m_fullPath = path;
    m_assetName = std::filesystem::path(m_fullPath).filename().string();
    m_assetDirectory = std::filesystem::path(m_fullPath).parent_path().string();
}
void Asset::set_fallback_name(AssetType type) {
    switch (type) {
        case AssetType::Texture:
            m_fallbackName = fallbackTextureName;
            break;
        case AssetType::Mesh:
            m_fallbackName = fallbackMeshName;
            break;
        case AssetType::Shader:
            m_fallbackName = fallbackShaderName;
            break;
        case AssetType::Cubemap:
            m_fallbackName = fallbackCubeMapName;
            break;
        case AssetType::Script:
            m_fallbackName = fallbackScriptName;
            break;
    }
}
}  // namespace knot
