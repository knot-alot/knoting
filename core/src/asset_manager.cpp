#include <knoting/asset_manager.h>

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <sys/types.h>
#include <unistd.h>
#endif

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

    const std::string fallbackMesh = "fallbackMesh";
    m_assets.insert({fallbackMesh, std::make_shared<components::Mesh>()});
    std::static_pointer_cast<components::Mesh>(m_assets[fallbackMesh])->generate_default_asset();

    const std::string primitivePlane = "postProcessPlane";
    m_assets.insert({primitivePlane, std::make_shared<components::Mesh>()});
    std::static_pointer_cast<components::Mesh>(m_assets[primitivePlane])->generate_post_process_plane();

    //=From File======
    AssetManager::load_asset<components::Texture>("UV_Grid_test.png");
    AssetManager::load_asset<components::Texture>("normal_tiles_1k.png");
    AssetManager::load_asset<components::Texture>("skybox/cmft_skybox.hdr");
    AssetManager::load_asset<components::Texture>("skybox/cmtr_radiance.hdr");
    AssetManager::load_asset<components::Texture>("skybox/cmtr_irradiance.hdr");

    AssetManager::load_asset<components::Mesh>("primitive_plane.obj");
    AssetManager::load_asset<components::Mesh>("uv_cube.obj");
    AssetManager::load_asset<components::Mesh>("dragon.obj");
}

void AssetManager::load_assets_serialize() {}

std::filesystem::path AssetManager::get_resources_path() {
    std::filesystem::path path = get_executable_path();
    return path.parent_path().parent_path().append("res/");
}

std::filesystem::path AssetManager::get_executable_path() {
    std::array<char, 512> buffer;
    std::fill(buffer.begin(), buffer.end(), '\0');

#if defined(WIN32) || defined(_WIN32)
    GetModuleFileName(nullptr, buffer.data(), MAX_PATH);
#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", buffer.data(), buffer.size());
#endif

    std::string result = std::string(buffer.begin(), buffer.end());
    std::replace(result.begin(), result.end(), '\\', '/');

    return result;
}

}  // namespace knot
