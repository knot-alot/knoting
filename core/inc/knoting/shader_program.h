#pragma once

#include <bgfx/bgfx.h>
#include <string>

namespace knot {
class ShaderProgram {
   public:
    ShaderProgram();
    ~ShaderProgram();

    bool load_shader(const std::string& folderName,
                     const std::string& vertexShaderPath,
                     const std::string& fragmentShaderPath);
    bgfx::ProgramHandle get_program() { return m_program; }

   private:
    void create_program(std::string& vertexShaderPath, std::string& fragmentShaderPath);

    bool does_file_path_exist(const std::string& path);
    const bgfx::Memory* bgfx_load_memory(const char* filename);

    std::string get_cross_platform_path(const std::string& folderName, const std::string& fileName);

   private:
    bgfx::ProgramHandle m_program;
};

}  // namespace knot
