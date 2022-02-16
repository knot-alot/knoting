#include <knoting/asset_manager.h>

namespace knot {

void AssetManager::on_awake() {
    s_assetManager = std::ref(*this);
    // TODO LOAD ALL ASSETS IN "ASSET/RES" FOLDER
    load_assets_manual();
    load_assets_serialize();
}
void AssetManager::on_destroy() {
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        it->second->on_destroy();
    }
    log::info("AssetManager destroyed");
}

void AssetManager::load_assets_manual() {
    //=Gen Textures===
    const std::string fb_tex = "fallbackTexture";
    m_assets.insert({fb_tex, std::make_shared<components::Texture>()});
    std::static_pointer_cast<components::Texture>(m_assets[fb_tex])->generate_default_asset();

    const std::string white_tex = "whiteTexture";
    m_assets.insert({white_tex, std::make_shared<components::Texture>()});
    std::static_pointer_cast<components::Texture>(m_assets[white_tex])
        ->generate_solid_color_texture(vec4(1), white_tex);

    //=Gen Meshes=====
    const std::string fb_msh = "fallbackMesh";
    m_assets.insert({fb_msh, std::make_shared<components::Mesh>()});
    std::static_pointer_cast<components::Texture>(m_assets[fb_msh])->generate_default_asset();

    //=From File======
    AssetManager::load_asset<components::Texture>("UV_Grid_test.png");
    AssetManager::load_asset<components::Texture>("normal_tiles_1k.png");

    AssetManager::load_asset<components::Mesh>("uv_cube.obj");
    AssetManager::load_asset<components::Mesh>("dragon.obj");
}

void AssetManager::load_assets_serialize() {}
}  // namespace knot
