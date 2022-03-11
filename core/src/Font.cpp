#include <knoting/Font.h>

namespace knot {
namespace components {
Font::Font() {}
Font::~Font() {}
void Font::on_awake() {
    m_fontManager = new FontManager(512);
    m_textBufferManager = new TextBufferManager(m_fontManager);
     m_fontFiles = loadTtf(m_fontManager, "../res/textures/droidsans.ttf");

    m_fonts = m_fontManager->createFontByPixelSize(m_fontFiles, 0, 25);
    m_fontManager->preloadGlyph(m_fonts, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. \n");
    m_staticText = m_textBufferManager->createTextBuffer(FONT_TYPE_DISTANCE_SUBPIXEL, BufferType::Static);
    m_textBufferManager->setPenPosition(m_staticText, -70.0f, -50.0f);

    m_textBufferManager->appendText(m_staticText, m_fonts, L"Knoting Game\n");
}

void* Font::load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size) {
    if (bx::open(_reader, _filePath)) {
        uint32_t size = (uint32_t)bx::getSize(_reader);
        void* data = BX_ALLOC(_allocator, size);
        bx::read(_reader, data, size, bx::ErrorAssert{});
        bx::close(_reader);
        if (NULL != _size) {
            *_size = size;
        }
        return data;
    } else {
        std::printf("Failed to open: %s.", _filePath);
    }

    if (NULL != _size) {
        *_size = 0;
    }

    return NULL;
}

void* Font::load(const char* _filePath, uint32_t* _size) {
    return load(entry::getFileReader(), entry::getAllocator(), _filePath, _size);
}

TrueTypeHandle Font::loadTtf(FontManager* _fm, const char* _filePath) {
    uint32_t size;
    void* data = Font::load(_filePath, &size);

    if (NULL != data) {
        TrueTypeHandle handle = _fm->createTtf((uint8_t*)data, size);
        BX_FREE(entry::getAllocator(), data);
        return handle;
    }

    TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
    return invalid;

}

};  // namespace components
}  // namespace knot
