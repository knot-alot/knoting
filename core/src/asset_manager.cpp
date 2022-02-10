#include <knoting/asset_manager.h>

namespace knot {

void AssetManager::on_awake() {
    s_assetManager = std::ref(*this);
    // TODO LOAD ALL ASSETS IN "ASSET/RESOURCE" FOLDER
    // MANUALLY LOAD FOR NOW
    load_assets_manual();

    load_assets_serialize();
}
void AssetManager::on_destroy() {
    Subsystem::on_destroy();
}

void AssetManager::load_assets_manual() {
    auto assetManager = get_asset_manager()->get();

    {
        auto alb = assetManager.load_asset<components::Texture>("UV_Grid_test.png").lock();
        auto nrm = assetManager.load_asset<components::Texture>("normal_tiles_1k.png").lock();
    }
    {
        auto alb = assetManager.load_asset<components::Texture>("UV_Grid_test.png").lock();
        auto nrm = assetManager.load_asset<components::Texture>("normal_tiles_1k.png").lock();
    }
}

void AssetManager::load_assets_serialize() {}

}  // namespace knot
