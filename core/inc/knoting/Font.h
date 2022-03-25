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
#include <knoting/component.h>
#include <knoting/entry.h>
#include <cereal/cereal.hpp>

namespace knot {
namespace components {

class Font : public Component<Font> {
   public:
    Font();
    ~Font();
    void on_awake();
    void set_PenPosition(float x, float y);
    void set_text(char* text);

    float get_penX() { return m_penX; }
    float get_penY() { return m_penY; }
    void* load(const char* _filePath, uint32_t* _size);
    void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
    TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath);
    TextBufferHandle get_text() { return m_staticText; }
    TextBufferManager* get_textBuffer() { return m_textBufferManager; }

    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_penX), CEREAL_NVP(m_penX), CEREAL_NVP(m_text));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_penX), CEREAL_NVP(m_penX), CEREAL_NVP(m_text));
        on_awake();
    }

   private:
    float m_penX = -70.0f;
    float m_penY = -50.0f;
    char* m_text = "Knoting Game";
    FontManager* m_fontManager;
    TextBufferManager* m_textBufferManager;
    TrueTypeHandle m_fontFiles;
    FontHandle m_fonts;
    TextBufferHandle m_staticText;
};
}  // namespace components
}  // namespace knot
