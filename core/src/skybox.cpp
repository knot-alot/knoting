#include "knoting/skybox.h"

namespace knot {
namespace components {

struct PosColorTexCoord0Vertex {
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_rgba;
    float m_u;
    float m_v;

    static void init() {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;
static float s_texelHalf = 0.0f;

void SkyBox::on_awake() {
    // TODO pass in shader
    m_shader.load_shader("skyboxport", "vs_skybox.bin", "fs_skybox.bin");
    // end TODO

    // clang-format off

    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox]          = bgfx::createUniform("s_texCube",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance]      = bgfx::createUniform("s_texCubeIrr",    bgfx::UniformType::Sampler);
    m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance]        = bgfx::createUniform("m_metallic",  bgfx::UniformType::Sampler);

    m_uniformSkyboxData = bgfx::createUniform("m_skyboxData",    bgfx::UniformType::Vec4);

    m_albedo    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::SkyBox]).lock();
    m_normal    = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::Irradiance]).lock();
    m_metallic  = AssetManager::load_asset<components::Texture>(m_textureSlotPath[(int)SkyBoxTextureHandle::Radiance]).lock();

    m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]           = m_albedo->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]       = m_normal->get_texture_handle();
    m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]         = m_metallic->get_texture_handle();

    PosColorTexCoord0Vertex::init();
    // clang-format on
}

void SkyBox::on_destroy() {
    bgfx::destroy(m_uniformSkyboxData);

    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance]);
    bgfx::destroy(m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance]);

    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]);
    bgfx::destroy(m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]);
}

SkyBox::SkyBox() {
    m_uniformSkyboxData = BGFX_INVALID_HANDLE;

    for (size_t i = 0; i < (size_t)SkyBoxUniformSamplerHandle::LAST; ++i) {
        m_uniformSamplerHandle[i] = BGFX_INVALID_HANDLE;
    }

    for (size_t i = 0; i < (size_t)SkyBoxTextureHandle::LAST; ++i) {
        m_textureHandles[i] = BGFX_INVALID_HANDLE;
    }
}

SkyBox::~SkyBox() {}
void SkyBox::set_uniforms() {
    m_skyboxData.x = m_exposure;
    m_skyboxData.y = (int)m_backgroundType;
    m_skyboxData.z = 0.0f;
    m_skyboxData.w = 0.0f;

    bgfx::setUniform(m_uniformSkyboxData, &m_skyboxData);

    bgfx::setTexture(0, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::SkyBox],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::SkyBox]);
    bgfx::setTexture(1, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Irradiance],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::Irradiance]);
    bgfx::setTexture(2, m_uniformSamplerHandle[(size_t)SkyBoxUniformSamplerHandle::Radiance],
                     m_textureHandles[(size_t)SkyBoxTextureHandle::Radiance]);

    // clang-format off
}
void SkyBox::set_texture_slot_path(SkyBoxTextureType slot, const std::string& path){
switch (slot) {
        case SkyBoxTextureType::Albedo:
            break;
        case SkyBoxTextureType::Irradiance:
            break;
        case SkyBoxTextureType::Radiance:
            break;
        default:
            return;
    }
    m_textureSlotPath[(int)slot] = path;
}



void SkyBox::screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft, float _width, float _height){
//if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_layout) )
//	{
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_layout);
		PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx =  _width;
		const float miny = 0.0f;
		const float maxy = _height*2.0f;

		const float texelHalfW = 1/_textureWidth;
		const float texelHalfH = 1/_textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu =  1.0f + texelHalfW;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			std::swap(minv, maxv);
			minv -= 1.0f;
			maxv -= 1.0f;
		}

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_rgba = 0xffffffff;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_rgba = 0xffffffff;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_rgba = 0xffffffff;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(0, &vb);
//	}
}

}  // namespace components
}  // namespace knot
