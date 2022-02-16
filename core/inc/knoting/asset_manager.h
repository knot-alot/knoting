#pragma once
#include <knoting/assert.h>
#include <knoting/asset.h>
#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <uuid.h>

#include <knoting/mesh.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>

namespace knot {
using namespace asset;

class AssetManager : public Subsystem {
   public:
    AssetManager() = default;
    AssetManager(const AssetManager& other) = delete;
    void on_awake() override;
    void on_destroy() override;

    void load_assets_manual();
    void load_assets_serialize();

    static std::optional<std::reference_wrapper<AssetManager>> get_asset_manager() { return s_assetManager; };

   private:
    // TODO consider tuple / UUID for when assets are loaded via serialization
    std::map<std::string, std::shared_ptr<Asset>> m_assets;

    inline static std::optional<std::reference_wrapper<AssetManager>> s_assetManager = std::nullopt;

   private:
    template <typename T>
    inline std::weak_ptr<T> internal_load_asset(const std::string& path) {
        if (!std::is_base_of<Asset, T>::value) {
            log::error("ASSET : " + path + " IS NOT OF BASE CLASS ASSET");
        }
        auto iterator = m_assets.find(path);
        if (iterator == m_assets.end()) {
            log::debug("Asset : " + path + " is being loaded");
            std::shared_ptr<T> tempAsset = std::make_shared<T>(path);
            tempAsset.get()->on_awake();
            if (tempAsset.get()->get_asset_state() == AssetState::Failed) {
                log::warn("Asset manager failed to load {} loading fallback");
                std::shared_ptr<T> result = std::static_pointer_cast<T>(m_assets[tempAsset.get()->get_fallback_name()]);
                return result;
            } else {
                log::info("adding asset: {}", path);
                m_assets.insert({path, tempAsset});
                std::shared_ptr<T> result = std::static_pointer_cast<T>(m_assets[path]);
                result.get()->on_awake();
                return result;
            }
        }

        auto r = std::static_pointer_cast<T>(m_assets[path]);
        r->on_awake();
        return r;
    }

    inline std::weak_ptr<components::Mesh> internal_load_asset(const std::string& path) {
        if (!std::is_base_of<Asset, components::Mesh>::value) {
            log::error("ASSET : " + path + " IS NOT OF BASE CLASS ASSET");
        }
        auto iterator = m_assets.find(path);
        if (iterator == m_assets.end()) {
            log::debug("Asset : " + path + " is being loaded");
            components::Mesh tempAsset = components::Mesh(path);
            tempAsset.on_awake();
            if (tempAsset.get_asset_state() == AssetState::Failed) {
                log::warn("ASSET MANAGER FAILED TO LOAD");
                std::shared_ptr<components::Mesh> result =
                    std::static_pointer_cast<components::Mesh>(m_assets[tempAsset.get_fallback_name()]);
                return result;
            } else {
                log::info("adding asset: {}", path);
                m_assets.insert({path, std::make_shared<components::Mesh>(tempAsset)});
                std::shared_ptr<components::Mesh> result = std::static_pointer_cast<components::Mesh>(m_assets[path]);
                result.get()->on_awake();
                return result;
            }
        }
        log::info("asset [ {} ] was found", path);
        auto r = std::static_pointer_cast<components::Mesh>(m_assets[path]);
        r->on_awake();
        return r;
    }

   public:
    template <typename T>
    inline static std::weak_ptr<T> load_asset(const std::string& path) {
        auto managerOpt = get_asset_manager();
        KNOTING_ASSERT_MESSAGE(managerOpt.has_value(), "ASSET MANAGER IS EMPTY")
        auto& assetManager = managerOpt->get();
        return assetManager.internal_load_asset<T>(path);
    }
};
}  // namespace knot
