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
    const std::string fb_tex = "fallbackTexture";
    m_assets.insert({fb_tex, std::make_shared<components::Texture>()});
    std::static_pointer_cast<components::Texture>(m_assets[fb_tex])->generate_default_asset();
    // SOLID COLOR TEXTURES
    const std::string white_tex = "whiteTexture";
    m_assets.insert({white_tex, std::make_shared<components::Texture>()});
    std::static_pointer_cast<components::Texture>(m_assets[white_tex])
        ->generate_solid_color_texture(vec4(1), white_tex);
    // FALLBACK MESH
    // FALLBACK SHADER
    // FROM FILE
    AssetManager::load_asset<components::Texture>("UV_Grid_test.png");
    AssetManager::load_asset<components::Texture>("normal_tiles_1k.png");
}

void AssetManager::load_assets_serialize() {}
}  // namespace knot
