#include <NoesisPCH.h>
#include <bgfx/bgfx.h>
#include <string>
#include <NsRender/RenderDevice.h>
#include <NsRender/RenderTarget.h>
#include <NsRender/Texture.h>
#include <NsRender/RenderDeviceApi.h>
using namespace Noesis;
class BGFXTexture : public Noesis::Texture {
   private:
    const uint32_t width;
    const uint32_t height;
    const bool inverted;

   public:
    const size_t stride;
    const bgfx::TextureHandle handle;

    BGFXTexture(bgfx::TextureHandle handle, uint32_t width, uint32_t height, size_t stride, bool inverted)
        : handle(handle), width(width), height(height), stride(stride), inverted(inverted){};

    ~BGFXTexture() { bgfx::destroy(handle); }

    // Returns the width of the texture
    virtual uint32_t GetWidth() const override { return width; }

    // Returns the height of the texture
    virtual uint32_t GetHeight() const override { return height; }

    // True if the texture has mipmaps
    virtual bool HasMipMaps() const override { return false; }

    // True is the texture must be vertically inverted when mapped. This is true for render targets
    // on platforms (OpenGL) where texture V coordinate is zero at the "bottom of the texture"
    virtual bool IsInverted() const override { return inverted; }
};

class BGFXRenderTarget : public Noesis::RenderTarget {
   private:
    Noesis::Ptr<BGFXTexture> texture;

   public:
    bgfx::TextureHandle stencilHandle;
    bgfx::FrameBufferHandle fboHandle;
    uint32_t width;
    uint32_t height;

    BGFXRenderTarget(uint32_t width, uint32_t height);
    ~BGFXRenderTarget() {
        if (bgfx::isValid(fboHandle))
            bgfx::destroy(fboHandle);
    }
    virtual Noesis::Texture* GetTexture() override { return texture; }
};

struct NoesisShader {
    Shader shader;
    int vertexFormat;
    std::string shaderName;
};

struct VBO {
    bgfx::DynamicVertexBufferHandle handle;
    int stride;
};

class BGFXRenderDevice : public Noesis::RenderDevice {
   private:
    Noesis::DeviceCaps caps;

    float identityMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    bgfx::ViewId viewId;
    bgfx::ViewId maxViewId;
    bgfx::ViewId activeViewId;

    unsigned char* vboData;
    bgfx::VertexLayout vertexLayouts[32];
    uint32_t vboOffset;

    BGFXRenderTarget* activeRenderTarget;

    bgfx::IndexBufferHandle clearIbo;
    bgfx::DynamicVertexBufferHandle clearVbo;
    float clearData[8];
    int clearShaderIndex;

    unsigned char* iboData;
    uint32_t iboOffset;

    uint32_t width;
    uint32_t height;

    NoesisShader programs[52];

    uint32_t getWrapSMode(Noesis::SamplerState sampler) const;
    uint32_t getWrapTMode(Noesis::SamplerState sampler) const;
    uint32_t getMinMode(Noesis::SamplerState sampler) const;
    uint32_t getMagMode(Noesis::SamplerState sampler) const;
    uint32_t getSamplerFlags(Noesis::SamplerState sampler) const;

   public:
    BGFXRenderDevice(bgfx::ViewId viewId);

    // Resets view counter
    void StartFrame();

    // From RenderDevice
    //@{
    // Retrieves device render capabilities
    const Noesis::DeviceCaps& GetCaps() const override;
  
    // Creates render target surface with given dimensions and number of samples
    Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label,
                                                         uint32_t width,
                                                         uint32_t height,
                                                         uint32_t sampleCount,
                                                         bool needsStencil) override;

    // Creates render target sharing transient (stencil, colorAA) buffers with the given surface
    Noesis::Ptr<Noesis::RenderTarget> CloneRenderTarget(const char* label, Noesis::RenderTarget* surface) override;

    // Creates texture with given dimensions and format. For immutable textures, the content of
    // each mipmap is given in 'data'. The passed data is tightly packed (no extra pitch). When
    // 'data' is null the texture is considered dynamic and will be updated using UpdateTexture()
    Noesis::Ptr<Noesis::Texture> CreateTexture(const char* label,
                                               uint32_t width,
                                               uint32_t height,
                                               uint32_t numLevels,
                                               Noesis::TextureFormat::Enum format,
                                               const void** data) override;

    // Updates texture mipmap copying the given data to desired position. The passed data is
    // tightly packed (no extra pitch). Origin is located at the left of the first scanline
    void UpdateTexture(Noesis::Texture* texture,
                       uint32_t level,
                       uint32_t x,
                       uint32_t y,
                       uint32_t width,
                       uint32_t height,
                       const void* data) override;

    // Begins rendering offscreen or onscreen commands
    void BeginOffscreenRender() override;

    // Binds render target and sets viewport to cover the entire surface
    void SetRenderTarget(Noesis::RenderTarget* surface) override;

    // Clears the given region to transparent (#000000) and sets the scissor rectangle to fit it.
    // Until next call to EndTile() all rendering commands will only update the extents of the tile
    void BeginTile(const Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight) override;

    // Completes rendering to the tile specified by BeginTile()
    void EndTile() override;

    // Resolves multisample render target
    void ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, uint32_t numTiles) override;

    // Ends rendering
    void EndRender() override;

    // Gets a pointer to stream vertices
    void* MapVertices(uint32_t bytes) override;

    // Invalidates the pointer previously mapped
    void UnmapVertices() override;

    // Gets a pointer to stream 16-bit indices
    void* MapIndices(uint32_t bytes) override;

    // Invalidates the pointer previously mapped
    void UnmapIndices() override;

    // Draws primitives for the given batch
    void DrawBatch(const Noesis::Batch& batch) override;
    //@}

    void setSize(uint32_t width, uint32_t height);
};

BGFXRenderTarget::BGFXRenderTarget(uint32_t width, uint32_t height) : width(width), height(height) {
    texture =
        *new BGFXTexture(bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT),
                         width, height, 4, true);
}

BGFXRenderDevice::BGFXRenderDevice(bgfx::ViewId viewId) : viewId(viewId) {
    bgfx::setViewName(viewId, "Noesis");
    bgfx::setViewMode(viewId, bgfx::ViewMode::Sequential);

    unsigned short ibo[6] = {0, 1, 2, 2, 3, 0};
    clearIbo = bgfx::createIndexBuffer(bgfx::copy(ibo, 12));
    bgfx::VertexLayout clearLayout;
    clearLayout.begin();
    clearLayout.add(bgfx::Attrib::Enum::Position, 2, bgfx::AttribType::Enum::Float, false, false);
    clearLayout.end();
    clearVbo = bgfx::createDynamicVertexBuffer(4, clearLayout);
    clearShaderIndex = 0;

    // Setup buffers
    iboData = new unsigned char[DYNAMIC_IB_SIZE];
    vboData = new unsigned char[DYNAMIC_VB_SIZE];

    // create VBOs for permutations of vertex format
    for (uint32_t i = 0; i < 32; i++) {
        bgfx::VertexLayout layout;
        layout.begin();
        layout.add(bgfx::Attrib::Enum::Position, 2, bgfx::AttribType::Enum::Float, false, false);

        if ((i & VFColor) != 0)
            layout.add(bgfx::Attrib::Enum::Color0, 4, bgfx::AttribType::Enum::Uint8, true, false);
        if ((i & VFTex0) != 0)
            layout.add(bgfx::Attrib::Enum::TexCoord0, 2, bgfx::AttribType::Enum::Float, false, false);
        if ((i & VFTex1) != 0)
            layout.add(bgfx::Attrib::Enum::TexCoord1, 2, bgfx::AttribType::Enum::Float, false, false);
        if ((i & VFTex2) != 0)
            layout.add(bgfx::Attrib::Enum::TexCoord2, 4, bgfx::AttribType::Enum::Int16, false, false);
        if ((i & VFCoverage) != 0)
            layout.add(bgfx::Attrib::Enum::TexCoord3, 1, bgfx::AttribType::Enum::Float, false, false);
        layout.end();

        vertexLayouts[i] = layout;
    }

    // Setup programs
    struct Program {
        uint8_t vertexFormat;
        int8_t vShaderIdx;
        int8_t fShaderIdx;
    };

    const std::string vertexShaders[] = {
        "Pos",         "PosColor",         "PosTex0",         "PosColorCoverage", "PosTex0Coverage", "PosColorTex1",
        "PosTex0Tex1", "PosColorTex1Tex2", "PosTex0Tex1Tex2", "PosColorTex1_SDF", "PosTex0Tex1_SDF",
    };

    const std::string fragmentShaders[] = {"RGBA",
                                           "Mask",
                                           "PathSolid",
                                           "PathLinear",
                                           "PathRadial",
                                           "PathPattern",
                                           "PathAASolid",
                                           "PathAALinear",
                                           "PathAARadial",
                                           "PathAAPattern",
                                           "SDFSolid",
                                           "SDFLinear",
                                           "SDFRadial",
                                           "SDFPattern",
                                           "ImageOpacitySolid",
                                           "ImageOpacityLinear",
                                           "ImageOpacityRadial",
                                           "ImageOpacityPattern",
                                           "ImageShadow35V",
                                           "ImageShadow63V",
                                           "ImageShadow127V",
                                           "ImageShadow35HSolid",
                                           "ImageShadow35HLinear",
                                           "ImageShadow35HRadial",
                                           "ImageShadow35HPattern",
                                           "ImageShadow63HSolid",
                                           "ImageShadow63HLinear",
                                           "ImageShadow63HRadial",
                                           "ImageShadow63HPattern",
                                           "ImageShadow127HSolid",
                                           "ImageShadow127HLinear",
                                           "ImageShadow127HRadial",
                                           "ImageShadow127HPattern",
                                           "ImageBlur35V",
                                           "ImageBlur63V",
                                           "ImageBlur127V",
                                           "ImageBlur35HSolid",
                                           "ImageBlur35HLinear",
                                           "ImageBlur35HRadial",
                                           "ImageBlur35HPattern",
                                           "ImageBlur63HSolid",
                                           "ImageBlur63HLinear",
                                           "ImageBlur63HRadial",
                                           "ImageBlur63HPattern",
                                           "ImageBlur127HSolid",
                                           "ImageBlur127HLinear",
                                           "ImageBlur127HRadial",
                                           "ImageBlur127HPattern"};

    const Program programConfig[] = {
        {VFPos, 0, 0},                               // RGBA
        {VFPos, 0, 1},                               // Mask
        {VFPos | VFColor, 1, 2},                     // PathSolid
        {VFPos | VFTex0, 2, 3},                      // PathLinear
        {VFPos | VFTex0, 2, 4},                      // PathRadial
        {VFPos | VFTex0, 2, 5},                      // PathPattern
        {VFPos | VFColor | VFCoverage, 3, 6},        // PathAASolid
        {VFPos | VFTex0 | VFCoverage, 4, 7},         // PathAALinear
        {VFPos | VFTex0 | VFCoverage, 4, 8},         // PathAARadial
        {VFPos | VFTex0 | VFCoverage, 4, 9},         // PathAAPattern
        {VFPos | VFColor | VFTex1, 9, 10},           // SDFSolid
        {VFPos | VFTex0 | VFTex1, 10, 11},           // SDFLinear
        {VFPos | VFTex0 | VFTex1, 10, 12},           // SDFRadial
        {VFPos | VFTex0 | VFTex1, 10, 13},           // SDFPattern
        {0, -1, -1},                                 // SDFSolid
        {0, -1, -1},                                 // SDFLinear
        {0, -1, -1},                                 // SDFRadial
        {0, -1, -1},                                 // SDFPattern
        {VFPos | VFColor | VFTex1, 5, 14},           // ImageOpacitySolid
        {VFPos | VFTex0 | VFTex1, 6, 15},            // ImageOpacityLinear
        {VFPos | VFTex0 | VFTex1, 6, 16},            // ImageOpacityRadial
        {VFPos | VFTex0 | VFTex1, 6, 17},            // ImageOpacityPattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 18},  // Image_Shadow35V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 19},  // Image_Shadow63V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 20},  // Image_Shadow127V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 21},  // Image_Shadow35H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 22},   // Image_Shadow35H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 23},   // Image_Shadow35H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 24},   // Image_Shadow35H_Pattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 25},  // Image_Shadow63H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 26},   // Image_Shadow63H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 27},   // Image_Shadow63H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 28},   // Image_Shadow63H_Pattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 29},  // Image_Shadow127H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 30},   // Image_Shadow127H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 31},   // Image_Shadow127H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 32},   // Image_Shadow127H_Pattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 33},  // Image_Blur35V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 34},  // Image_Blur63V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 35},  // Image_Blur127V
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 36},  // Image_Blur35H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 37},   // Image_Blur35H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 38},   // Image_Blur35H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 39},   // Image_Blur35H_Pattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 40},  // Image_Blur63H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 41},   // Image_Blur63H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 42},   // Image_Blur63H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 43},   // Image_Blur63H_Pattern
        {VFPos | VFColor | VFTex1 | VFTex2, 7, 44},  // Image_Blur127H_Solid
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 45},   // Image_Blur127H_Linear
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 46},   // Image_Blur127H_Radial
        {VFPos | VFTex0 | VFTex1 | VFTex2, 8, 47}    // Image_Blur127H_Pattern
    };

    for (uint32_t i = 0; i < NS_COUNTOF(programConfig); i++) {
        const Program& p = programConfig[i];

        if (p.vShaderIdx != -1 && p.fShaderIdx != -1) {
            programs[i].shader.init("sandal/noesis/vertex", "sandal/noesis/" + fragmentShaders[p.fShaderIdx]);
            programs[i].vertexFormat = p.vertexFormat;
            programs[i].shaderName = fragmentShaders[p.fShaderIdx];
            assert(p.vertexFormat < 32);
        }
    }
}

void BGFXRenderDevice::StartFrame() {
    maxViewId = viewId - 16;
}

const Noesis::DeviceCaps& BGFXRenderDevice::GetCaps() const {
    return caps;
}

Noesis::Ptr<Noesis::RenderTarget> BGFXRenderDevice::CreateRenderTarget(const char* label,
                                                                       uint32_t width,
                                                                       uint32_t height,
                                                                       uint32_t sampleCount,
                                                                       bool needsStencil) {
    Noesis::Ptr<BGFXRenderTarget> surface = *new BGFXRenderTarget(width, height);
    surface->stencilHandle = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24S8);
    bgfx::TextureHandle attachments[] = {((BGFXTexture*)surface->GetTexture())->handle, surface->stencilHandle};
    surface->fboHandle = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, false);

    return surface;
}

Noesis::Ptr<Noesis::RenderTarget> BGFXRenderDevice::CloneRenderTarget(const char* label, Noesis::RenderTarget* shared) {
    BGFXRenderTarget* sharedTarget = (BGFXRenderTarget*)shared;

    Noesis::Ptr<BGFXRenderTarget> surface = *new BGFXRenderTarget(sharedTarget->width, sharedTarget->height);
    bgfx::TextureHandle attachments[] = {((BGFXTexture*)surface->GetTexture())->handle, sharedTarget->stencilHandle};
    surface->fboHandle = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, false);

    bgfx::setViewFrameBuffer(maxViewId, surface->fboHandle);

    return surface;
}

Noesis::Ptr<Noesis::Texture> BGFXRenderDevice::CreateTexture(const char* label,
                                                             uint32_t width,
                                                             uint32_t height,
                                                             uint32_t numLevels,
                                                             Noesis::TextureFormat::Enum format,
                                                             const void** data) {
    assert(numLevels == 1);

    bgfx::TextureFormat::Enum textureFormat;
    size_t stride;
    switch (format) {
        case Noesis::TextureFormat::Enum::RGBA8:
            textureFormat = bgfx::TextureFormat::Enum::RGBA8;
            stride = 4;
            break;
        case Noesis::TextureFormat::Enum::R8:
            textureFormat = bgfx::TextureFormat::Enum::R8;
            stride = 1;
            break;
        default:
            abort();
    }
    return *new BGFXTexture(
        bgfx::createTexture2D(width, height, false, 1, textureFormat, BGFX_TEXTURE_RT,
                              data != nullptr ? bgfx::copy(data[0], width * height * stride) : NULL),
        width, height, stride, false);
}

void BGFXRenderDevice::UpdateTexture(Noesis::Texture* texture,
                                     uint32_t level,
                                     uint32_t x,
                                     uint32_t y,
                                     uint32_t width,
                                     uint32_t height,
                                     const void* data) {
    BGFXTexture* tex = (BGFXTexture*)texture;
    assert(level == 0);
    bgfx::updateTexture2D(tex->handle, 0, level, x, y, width, height, bgfx::copy(data, width * height * tex->stride));
}

void BGFXRenderDevice::BeginOffscreenRender() {
    // Reset
    vboOffset = 0;
    iboOffset = 0;
    activeRenderTarget = nullptr;
    bgfx::setViewRect(viewId, 0, 0, width, height);
    activeViewId = viewId;    
}

void BGFXRenderDevice::SetRenderTarget(Noesis::RenderTarget* surface) {
    activeRenderTarget = (BGFXRenderTarget*)surface;
}

void BGFXRenderDevice::BeginTile(const Noesis::Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight) {
    assert(activeRenderTarget);

    maxViewId++;
    assert(maxViewId < viewId);  // assert so i can debug once it happens eventually
    // if (maxViewId == viewId) maxViewId = viewId - 16;// maximum 16 views, wrap around maybe?!
    bgfx::setViewClear(maxViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_STENCIL, 0);
    std::string name("Noesis FBO " + std::to_string(maxViewId - viewId));
    bgfx::setViewName(maxViewId, name.c_str());
    bgfx::setViewMode(maxViewId, bgfx::ViewMode::Sequential);
    bgfx::setViewRect(maxViewId, 0, 0, surfaceWidth, surfaceHeight);
    bgfx::setViewScissor(maxViewId, tile.x, tile.y, tile.width, tile.height);
    bgfx::setViewTransform(
        maxViewId, identityMatrix,
        value_ptr(glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, 0.0f, 10.0f)));
    bgfx::setViewFrameBuffer(maxViewId, activeRenderTarget->fboHandle);

    activeViewId = maxViewId;
}

void BGFXRenderDevice::EndTile() {
    bgfx::setViewScissor(activeViewId, 0, 0, 0, 0);
}

void BGFXRenderDevice::ResolveRenderTarget(Noesis::RenderTarget* surface,
                                           const Noesis::Tile* tiles,
                                           uint32_t numTiles) {}

void BGFXRenderDevice::EndRender() {}

void* BGFXRenderDevice::MapVertices(uint32_t bytes) {
    vboOffset = bytes;
    return vboData;
}

void BGFXRenderDevice::UnmapVertices() {}

void* BGFXRenderDevice::MapIndices(uint32_t bytes) {
    iboOffset = bytes;
    return iboData;
}

void BGFXRenderDevice::UnmapIndices() {}

void BGFXRenderDevice::DrawBatch(const Noesis::Batch& batch) {
    NoesisShader const& shader = programs[batch.shader.v];

    bgfx::setViewTransform(activeViewId, identityMatrix, batch.projMtx);

    BGFXTexture* tex = (BGFXTexture*)(batch.glyphs ? batch.glyphs : batch.image);
    if (tex) {
        float dims[4] = {(float)tex->GetWidth(), (float)tex->GetHeight(), 1.0f / tex->GetWidth(),
                         1.0f / tex->GetHeight()};
        shader.shader.setUniform("u_textureDimensions"_hs, dims);
    }
    if (batch.opacity) {
        float opacity[4] = {*batch.opacity, *batch.opacity, *batch.opacity, *batch.opacity};
        shader.shader.setUniform("u_opacity"_hs, opacity);
    }
    if (batch.radialGrad)
        shader.shader.setUniform("u_radialGrad"_hs, batch.radialGrad, 2);
    if (batch.rgba)
        shader.shader.setUniform("u_rgba"_hs, batch.rgba);
    if (batch.effectParams)
        shader.shader.setUniform("u_effectParams"_hs, batch.effectParams, batch.effectParamsSize);

    bgfx::TransientVertexBuffer vbo;
    bgfx::TransientIndexBuffer ibo;

    // vertex data
    bgfx::VertexLayout const& layout = vertexLayouts[shader.vertexFormat];
    int v = bgfx::getAvailTransientVertexBuffer(batch.numVertices, layout);
    int i = bgfx::getAvailTransientIndexBuffer(batch.numIndices);
    bgfx::allocTransientVertexBuffer(&vbo, batch.numVertices, layout);
    memcpy(vbo.data, vboData + batch.vertexOffset, batch.numVertices * layout.getStride());

    bgfx::allocTransientIndexBuffer(&ibo, batch.numIndices);
    memcpy(ibo.data, iboData + batch.startIndex * sizeof(unsigned short), batch.numIndices * sizeof(unsigned short));

    bgfx::setVertexBuffer(0, &vbo, 0, batch.numVertices);
    bgfx::setIndexBuffer(&ibo, 0, batch.numIndices);

    // textures
    if (batch.pattern != 0)
        shader.shader.setTexture(0, "s_pattern"_hs, ((BGFXTexture*)batch.pattern)->handle,
                                 getSamplerFlags(batch.patternSampler));

    if (batch.ramps != 0)
        shader.shader.setTexture(1, "s_ramps"_hs, ((BGFXTexture*)batch.ramps)->handle,
                                 getSamplerFlags(batch.rampsSampler));

    if (batch.image != 0)
        shader.shader.setTexture(2, "s_image"_hs, ((BGFXTexture*)batch.image)->handle,
                                 getSamplerFlags(batch.imageSampler));

    if (batch.glyphs != 0)
        shader.shader.setTexture(3, "s_glyphs"_hs, ((BGFXTexture*)batch.glyphs)->handle,
                                 getSamplerFlags(batch.glyphsSampler));

    if (batch.shadow != 0)
        shader.shader.setTexture(4, "s_shadow"_hs, ((BGFXTexture*)batch.shadow)->handle,
                                 getSamplerFlags(batch.shadowSampler));

    // state
    uint32_t state = 0;
    uint32_t stencil = 0;
    if (batch.renderState.f.blendMode)
        state |=
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA) | BGFX_STATE_BLEND_EQUATION_ADD;
    if (batch.renderState.f.colorEnable)
        state |= BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
    if (batch.renderState.f.stencilMode) {
        switch ((Noesis::StencilMode::Enum)batch.renderState.f.stencilMode) {
            case Noesis::StencilMode::Disabled:
                break;
            case Noesis::StencilMode::Equal_Keep:
                stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_KEEP;
                break;
            case Noesis::StencilMode::Equal_Incr:
                stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_INCR;
                break;
            case Noesis::StencilMode::Equal_Decr:
                stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_DECR;
                break;
            default:
                abort();
        }
    }
    if (batch.stencilRef) {
        stencil |= BGFX_STENCIL_TEST_EQUAL | BGFX_STENCIL_FUNC_REF(batch.stencilRef) | BGFX_STENCIL_FUNC_RMASK(0xff);
    } else if (stencil != 0) {
        stencil |= BGFX_STENCIL_TEST_ALWAYS;
    }

    state |= BGFX_STATE_MSAA;

    bgfx::setStencil(stencil);
    bgfx::setState(state);
    bgfx::submit(activeViewId, shader.shader.handle);
}

void BGFXRenderDevice::setSize(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
}

uint32_t BGFXRenderDevice::getWrapSMode(Noesis::SamplerState sampler) const {
    switch (sampler.f.wrapMode) {
        // Clamp between 0.0 and 1.0
        case Noesis::WrapMode::Enum::ClampToEdge:
            return BGFX_SAMPLER_U_CLAMP;
            // Out of range texture coordinates return transparent zero (0,0,0,0)
        case Noesis::WrapMode::Enum::ClampToZero:
            return BGFX_SAMPLER_U_BORDER;
            // Wrap to the other side of the texture
        case Noesis::WrapMode::Enum::Repeat:
            return 0;
            // The same as repeat but flipping horizontally
        case Noesis::WrapMode::Enum::MirrorU:
            return BGFX_SAMPLER_U_MIRROR;
            // The same as repeat but flipping vertically
        case Noesis::WrapMode::Enum::MirrorV:
            return 0;
            // The combination of MirrorU and MirrorV
        case Noesis::WrapMode::Enum::Mirror:
            return BGFX_SAMPLER_U_MIRROR;
        default:
            abort();
            return 0;
    }
}

uint32_t BGFXRenderDevice::getWrapTMode(Noesis::SamplerState sampler) const {
    switch (sampler.f.wrapMode) {
        // Clamp between 0.0 and 1.0
        case Noesis::WrapMode::Enum::ClampToEdge:
            return BGFX_SAMPLER_V_CLAMP;
            // Out of range texture coordinates return transparent zero (0,0,0,0)
        case Noesis::WrapMode::Enum::ClampToZero:
            return BGFX_SAMPLER_V_BORDER;
            // Wrap to the other side of the texture
        case Noesis::WrapMode::Enum::Repeat:
            return 0;
            // The same as repeat but flipping horizontally
        case Noesis::WrapMode::Enum::MirrorU:
            return 0;
            // The same as repeat but flipping vertically
        case Noesis::WrapMode::Enum::MirrorV:
            return BGFX_SAMPLER_V_MIRROR;
            // The combination of MirrorU and MirrorV
        case Noesis::WrapMode::Enum::Mirror:
            return BGFX_SAMPLER_V_MIRROR;
        default:
            abort();
            return 0;
    }
}

uint32_t BGFXRenderDevice::getMinMode(Noesis::SamplerState sampler) const {
    switch (sampler.f.minmagFilter) {
        case Noesis::MinMagFilter::Nearest:
            switch (sampler.f.mipFilter) {
                case Noesis::MipFilter::Disabled:
                    return 0;
                case Noesis::MipFilter::Nearest:
                    return BGFX_SAMPLER_MIN_POINT;
                case Noesis::MipFilter::Linear:
                    return BGFX_SAMPLER_MIN_ANISOTROPIC;
                default:
                    abort();
                    return 0;
            }
        case Noesis::MinMagFilter::Linear:
            switch (sampler.f.mipFilter) {
                case MipFilter::Disabled:
                    return 0;
                case MipFilter::Nearest:
                    return BGFX_SAMPLER_MIN_POINT;
                case MipFilter::Linear:
                    return BGFX_SAMPLER_MIN_ANISOTROPIC;
                default:
                    abort();
                    return 0;
            }
        default:
            abort();
            return 0;
    }
}

uint32_t BGFXRenderDevice::getMagMode(Noesis::SamplerState sampler) const {
    switch (sampler.f.minmagFilter) {
        case Noesis::MinMagFilter::Nearest:
            return BGFX_SAMPLER_MAG_POINT;
        case Noesis::MinMagFilter::Linear:
            return BGFX_SAMPLER_MAG_ANISOTROPIC;
        default:
            abort();
            return 0;
    }
}

uint32_t BGFXRenderDevice::getSamplerFlags(Noesis::SamplerState sampler) const {
    return getWrapSMode(sampler) | getWrapTMode(sampler) | getMinMode(sampler) | getMagMode(sampler);
}
