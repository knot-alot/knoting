#include <knoting/log.h>
#include <knoting/mesh.h>
#include <filesystem>
#include <fstream>

namespace knot {
namespace components {

Mesh::Mesh() : Asset{AssetType::Mesh, "", "fallbackMesh"} {}
Mesh::Mesh(const std::string& path) : Asset{AssetType::Mesh, path, "fallbackMesh"} {}

Mesh::~Mesh() {}

void Mesh::on_awake() {
    //    create_cube();
    // internal_load_obj("TheStanfordDragon_83k.obj");
}

void Mesh::on_destroy() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
}

void Mesh::load_mesh(const std::string& localTexturePath) {
    std::string fullPath = PATH_MODELS + localTexturePath;
    std::filesystem::path fs_path = std::filesystem::path(fullPath);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    if (std::filesystem::exists(fs_path)) {
        log::debug("file loaded at : " + fullPath);
        internal_load_obj(fullPath);

    } else {
        log::error("file : " + fullPath + " does not exist");
    }
}

void Mesh::create_cube() {
    std::vector<int16_t> tempIndex = {
        0,  2,  1,  1,  2,  3,  4,  5,  6,  5,  7,  6,

        8,  10, 9,  9,  10, 11, 12, 13, 14, 13, 15, 14,

        16, 18, 17, 17, 18, 19, 20, 21, 22, 21, 23, 22,
    };

    m_indexBuffer = std::make_shared<IndexBuffer>();
    m_indexBuffer->set_index_buffer(tempIndex);

    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(&m_indexBuffer->get_index_start(), m_indexBuffer->get_memory_size()));

    VertexLayout::init();
    m_vertexLayout = {
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 1, 0},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 0, 1},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, 0.0f, 1.0f), 0, 1, 1},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0, 0},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 1, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 0, 1},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, 0.0f, -1.0f), 0, 1, 1},
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 1, 0},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 0, 1},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(0.0f, 1.0f, 0.0f), 0, 1, 1},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0, 0},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 1, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 0, 1},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(0.0f, -1.0f, 0.0f), 0, 1, 1},
        {1.0f, -1.0f, 1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0, 0},
        {1.0f, 1.0f, 1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 1, 0},
        {1.0f, -1.0f, -1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 0, 1},
        {1.0f, 1.0f, -1.0f, encode_normal_rgba8(1.0f, 0.0f, 0.0f), 0, 1, 1},
        {-1.0f, -1.0f, 1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0, 0},
        {-1.0f, 1.0f, 1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 1, 0},
        {-1.0f, -1.0f, -1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 0, 1},
        {-1.0f, 1.0f, -1.0f, encode_normal_rgba8(-1.0f, 0.0f, 0.0f), 0, 1, 1},
    };

    m_vbh =
        bgfx::createVertexBuffer(bgfx::makeRef(&m_vertexLayout[0], sizeof(m_vertexLayout[0]) * m_vertexLayout.size()),
                                 VertexLayout::s_meshVertexLayout);
}

void Mesh::generate_default_asset() {}

std::vector<std::string> split(std::string s, std::string t) {
    std::vector<std::string> res;
    while (1) {
        int pos = s.find(t);
        if (pos == -1) {
            res.push_back(s);
            break;
        }
        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1, s.size() - pos - 1);
    }
    return res;
}

bool Mesh::internal_load_obj(const std::string& fileName) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    if (fileName.find(".obj") != std::string::npos) {
        std::ifstream fin(fileName, std::ios::in);
        if (!fin) {
            log::error("cant open file {}", fileName);
            return false;
        }

        log::info("loading file {}", fileName);

        std::string lineBuffer;
        while (std::getline(fin, lineBuffer)) {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v") {
                glm::vec3 vertex;
                int dim = 0;
                while (dim < 3 && ss >> vertex[dim]) {
                    dim++;
                }
                tempVertices.push_back(vertex);

            } else if (cmd == "vt") {
                glm::vec2 uv;
                int dim = 0;
                while (dim < 2 && ss >> uv[dim]) {
                    dim++;
                }
                tempUVs.push_back(uv);

            } else if (cmd == "vn") {
                glm::vec3 normal;
                int dim = 0;
                while (dim < 3 && ss >> normal[dim]) {
                    dim++;
                }

                normal = glm::normalize(normal);
                tempNormals.push_back(normal);

            } else if (cmd == "f") {
                std::string faceData;
                int vertexIndex, uvIndex, normalIndex;  // v/vt/vn

                while (ss >> faceData) {
                    std::vector<std::string> data = split(faceData, "/");
                    // vertex index
                    if (data[0].size() > 0) {
                        sscanf_s(data[0].c_str(), "%d", &vertexIndex);
                        vertexIndices.push_back(vertexIndex);
                    }
                    // if the face vertex has a texture coord index
                    if (data.size() >= 1) {
                        if (data[1].size() > 0) {
                            sscanf_s(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }
                    // does the face vertex have a normal index
                    if (data.size() >= 2) {
                        if (data[2].size() > 0) {
                            sscanf_s(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
            }
        }

        // close file
        fin.close();

        log::info("finished Reading : {}", fileName);

        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            Vertex meshVertex;
            if (tempVertices.size() > 0) {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                meshVertex.position = vertex;
            }

            if (tempNormals.size() > 0) {
                glm::vec3 normal = tempNormals[normalIndices[i] - 1];
                meshVertex.normal = normal;
            }

            if (tempUVs.size() > 0) {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                meshVertex.texCoords = uv;
            }

            mVertices.push_back(meshVertex);
        }
    }

    VertexLayout::init();
    std::vector<int16_t> tempIndex;
    int ind = 0;
    for (auto v : mVertices) {
        log::info("U : {} , V : {}", (int16_t)v.texCoords.x, (int16_t)v.texCoords.y);
        tempIndex.emplace_back(ind);
        m_vertexLayout.emplace_back(VertexLayout{v.position.x, v.position.y, v.position.z,
                                                 encode_normal_rgba8(v.normal.x, v.normal.y, v.normal.z), 0,
                                                 v.texCoords.x, v.texCoords.y});
        ind++;
    }

    m_indexBuffer = std::make_shared<IndexBuffer>();
    m_indexBuffer->set_index_buffer(tempIndex);

    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(&m_indexBuffer->get_index_start(), m_indexBuffer->get_memory_size()));

    m_vbh =
        bgfx::createVertexBuffer(bgfx::makeRef(&m_vertexLayout[0], sizeof(m_vertexLayout[0]) * m_vertexLayout.size()),
                                 VertexLayout::s_meshVertexLayout);
    log::debug("init buffers {}", fileName);

    return true;
}

}  // namespace components
}  // namespace knot
