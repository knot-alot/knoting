#include <knoting/shader_program.h>

#include <knoting/log.h>
#include <filesystem>
#include <fstream>

namespace knot {

ShaderProgram::ShaderProgram() {}
ShaderProgram::~ShaderProgram() {
    bgfx::destroy(m_program);
}

bool ShaderProgram::load_shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::string fullVertexPath = get_cross_platform_path(vertexShaderPath);
    std::string fullFragmentPath = get_cross_platform_path(fragmentShaderPath);

    bool has_valid_paths;
    has_valid_paths = does_file_path_exist(fullVertexPath);
    has_valid_paths = does_file_path_exist(fullFragmentPath);

    create_program(fullVertexPath, fullFragmentPath);

    return has_valid_paths;
}

void ShaderProgram::create_program(std::string& fullVertexPath, std::string& fullFragmentPath) {
    bgfx::ShaderHandle vs = bgfx::createShader(bgfx_load_memory(fullVertexPath.c_str()));
    bgfx::ShaderHandle fs = bgfx::createShader(bgfx_load_memory(fullFragmentPath.c_str()));

    m_program = bgfx::createProgram(vs, fs);

    destroy(vs);
    destroy(fs);
}

const bgfx::Memory* ShaderProgram::bgfx_load_memory(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    const bgfx::Memory* mem = bgfx::alloc(uint32_t(size + 1));
    if (file.read((char*)mem->data, size)) {
        mem->data[mem->size - 1] = '\0';
        return mem;
    }
    return nullptr;
}

bool ShaderProgram::does_file_path_exist(const std::string& path) {
    std::filesystem::path vs_filesystem_path = std::filesystem::path(path);

    if (exists(vs_filesystem_path)) {
        log::debug("file loaded at : " + path);
    } else {
        log::error("file : " + path + " does not exist");
        return false;
    }

    return true;
}
std::string ShaderProgram::get_cross_platform_path(const std::string& path) {
    std::string shaderPath = "SHADER_BINARY_NOT_SET";
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:
            shaderPath = "../res/shaders/cubes/dx9/";
            break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            shaderPath = "../res/shaders/cubes/dx11/";
            break;
        case bgfx::RendererType::Gnm:
            break;
        case bgfx::RendererType::Metal:
            shaderPath = "../res/shaders/cubes/metal/";
            break;
        case bgfx::RendererType::OpenGL:
            shaderPath = "../res/shaders/cubes/glsl/";
            break;
        case bgfx::RendererType::OpenGLES:
            shaderPath = "../res/shaders/cubes/essl/";
            break;
        case bgfx::RendererType::Vulkan:
            shaderPath = "../res/shaders/cubes/spirv/";
            break;
        case bgfx::RendererType::Count:
            break;
    }

    shaderPath.append(path);
    return shaderPath;
}

}  // namespace knot