#pragma once

#include <string>
#include <bgfx/bgfx.h>

constexpr char PATH_SHADER[]  = "../res/shaders/";
constexpr char PATH_TEXTURE[] = "../res/textures/";
constexpr char PATH_MODELS[]  = "../res/models/";

namespace knot {

class Mesh {
   public:
    Mesh();
    ~Mesh();

    void load_mesh(const std::string& localTexturePath);
   private:


};

}  // namespace knot
