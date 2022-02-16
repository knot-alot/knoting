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
    if (m_assetState != AssetState::Finished) {
        m_assetState = AssetState::Loading;
        internal_load_obj(m_fullPath);
    }
}

void Mesh::on_destroy() {
    if (isValid(m_vbh)) {
        bgfx::destroy(m_vbh);
    }
    if (isValid(m_ibh)) {
        bgfx::destroy(m_ibh);
    }
    log::info("removed mesh : {}", m_fullPath);
}

void Mesh::create_cube() {
    std::vector<unsigned int> tempIndex = {
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

void Mesh::generate_default_asset() {
    m_assetState = AssetState::Loading;
    m_fullPath = "fallbackMesh";
    create_cube();
    if (!bgfx::isValid(m_vbh)) {
        log::error("VBH : fallbackMesh failed to be created");
        m_vbh = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
        return;
    }

    if (!bgfx::isValid(m_ibh)) {
        log::error("IBH : fallbackMesh failed to be created");
        m_ibh = BGFX_INVALID_HANDLE;
        m_assetState = AssetState::Failed;
        return;
    }

    m_assetState = AssetState::Finished;
    log::info("Fallback created");
}

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

bool Mesh::internal_load_obj(const std::string& path) {
    std::string fileName = PATH_MODELS + path;
    std::filesystem::path fs_path = fileName;

    if (!exists(fs_path)) {
        log::error(fileName + " - does not Exist");
        m_assetState = AssetState::Failed;
        return false;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    if (fileName.find(".obj") != std::string::npos) {
        std::ifstream fin(fileName, std::ios::in);
        if (!fin) {
            log::error("cant open file {}", fileName);
            m_vbh = BGFX_INVALID_HANDLE;
            m_ibh = BGFX_INVALID_HANDLE;
            m_assetState = AssetState::Failed;
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
                        sscanf(data[0].c_str(), "%d", &vertexIndex);
                        vertexIndices.push_back(vertexIndex);
                    }
                    // if the face vertex has a texture coord index
                    if (data.size() >= 1) {
                        if (data[1].size() > 0) {
                            sscanf(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }
                    // does the face vertex have a normal index
                    if (data.size() >= 2) {
                        if (data[2].size() > 0) {
                            sscanf(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
            }
        }

        // close file
        fin.close();

        log::info("finished Reading : {}", fileName);

        mVertices.resize(vertexIndices.size());
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
    for (auto v : mVertices) {
        m_vertexLayout.emplace_back(VertexLayout{v.position.x, v.position.y, v.position.z,
                                                 encode_normal_rgba8(v.normal.x, v.normal.y, v.normal.z), 0,
                                                 v.texCoords.x, v.texCoords.y});
    }
    m_ibh = BGFX_INVALID_HANDLE;
    m_vbh =
        bgfx::createVertexBuffer(bgfx::makeRef(&m_vertexLayout[0], sizeof(m_vertexLayout[0]) * m_vertexLayout.size()),
                                 VertexLayout::s_meshVertexLayout);
    log::debug("init buffers {}", fileName);
    m_assetState = AssetState::Finished;
    return true;
}

}  // namespace components
}  // namespace knot
