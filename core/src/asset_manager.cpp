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
    log::warn("ASSET MANAGER HAS BEEN DESTROYED");
}

void AssetManager::load_assets_manual() {
}

void AssetManager::load_assets_serialize() {
}
void AssetManager::on_update(double m_delta_time) {

    auto alb = AssetManager::load_asset<components::Texture>("UV_Grid_test.png").lock();
    auto nrm = AssetManager::load_asset<components::Texture>("normal_tiles_1k.png").lock();

    auto alb_2 = AssetManager::load_asset<components::Texture>("UV_Grid_test.png").lock();
    auto nrm_2 = AssetManager::load_asset<components::Texture>("normal_tiles_1k.png").lock();

    log::debug("ASSET CACHE SIZE");
    log::debug(m_assets.size());
    log::debug("-----------------------");
}

}  // namespace knot
