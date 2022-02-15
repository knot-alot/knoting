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
    const std::string fb_msh = "fallbackMesh";
    m_assets.insert({fb_msh, std::make_shared<components::Mesh>()});
    std::static_pointer_cast<components::Texture>(m_assets[fb_msh])->generate_default_asset();

    // FALLBACK SHADER
    // FROM FILE
    AssetManager::load_asset<components::Texture>("UV_Grid_test.png");
    AssetManager::load_asset<components::Texture>("normal_tiles_1k.png");
    AssetManager::load_asset<components::Mesh>("uv_cube.obj");

    for (auto as : m_assets) {
        log::debug("asset loaded manual : {}", as.first);
    }
}

void AssetManager::load_assets_serialize() {}
}  // namespace knot
