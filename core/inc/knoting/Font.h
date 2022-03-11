#pragma once

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/string.h>
#include <bx/timer.h>

#include <fstream>
#include "knoting/font_manager.h"
#include "knoting/text_buffer_manager.h"

#include <wchar.h>

#include <bgfx/bgfx.h>
#include <knoting/components.h>
#include <knoting/entry.h>

namespace knot {
namespace components {


class Font {
   public:
    Font();
    ~Font();
    void on_awake();
    void on_destroy();
    void* load(const char* _filePath, uint32_t* _size);
    void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
    TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath);
    TextBufferHandle get_text() { return m_staticText; }
    TextBufferManager* get_textBuffer() { return m_textBufferManager; }



   private:
    FontManager* m_fontManager;
    TextBufferManager* m_textBufferManager;
    TrueTypeHandle m_fontFiles;
    FontHandle m_fonts;
    TextBufferHandle m_staticText;
};
}  // namespace components
}  // namespace knot
