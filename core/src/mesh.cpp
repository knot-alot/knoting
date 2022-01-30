
#include <knoting/mesh.h>
#include <knoting/log.h>
#include <filesystem>



namespace knot{

Mesh::Mesh() {}

Mesh::~Mesh() {}

void knot::Mesh::load_mesh(const std::string& localTexturePath) {

    std::string fullPath = PATH_MODELS + localTexturePath;
    std::filesystem::path fs_path = std::filesystem::path(fullPath);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    uint16_t size;
    char* raw_image;

    if (exists(fs_path)) {
        log::debug("file loaded at : " + fullPath);

    } else {
        log::error("file : " + fullPath + " does not exist");
    }



}

}
