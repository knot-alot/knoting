#include <knoting/asset_manager.h>

namespace knot {

void AssetManager::on_awake() {
    log::warn("ON AWAKE");
    s_assetManager = std::ref(*this);
    // TODO LOAD ALL ASSETS IN "ASSET/RESOURCE" FOLDER
    // MANUALLY LOAD FOR NOW
    load_assets_manual();

    load_assets_serialize();
}
void AssetManager::on_destroy() {
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        it->second->on_destroy();
    }

    log::info("ASSET MANAGER HAS BEEN DESTROYED");
}

void AssetManager::load_assets_manual() {
    // FALLBACK TEXTURE
    m_assets.insert({"fallbackTexture", std::make_shared<components::Texture>()});
    std::static_pointer_cast<components::Texture>(m_assets["fallbackTexture"])->generate_default_asset();
    // FALLBACK MESH
    // FALLBACK SHADER
    // FROM FILE
    AssetManager::load_asset<components::Texture>("UV_Grid_test.png");
    AssetManager::load_asset<components::Texture>("normal_tiles_1k.png");
}

void AssetManager::load_assets_serialize() {}
}  // namespace knot
