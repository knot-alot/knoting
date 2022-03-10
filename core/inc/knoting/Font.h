/*#pragma once

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
namespace knot {
namespace components {
static bx::FileReaderI* s_fileReader = NULL;
bx::AllocatorI* g_allocator = getDefaultAllocator();
class Font {
   public:
    Font();
    ~Font();
    //=For ECS========
    void on_awake();
    void on_destroy();
    //================
    TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath){
        uint32_t size;
        void* data = load(_filePath, &size);

        if (NULL != data) {
            TrueTypeHandle handle = _fm->createTtf((uint8_t*)data, size);
            BX_FREE(getAllocator(), data);
            return handle;
        }

        TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
        return invalid;
    };
    void* load(const char* _filePath, uint32_t* _size){
        return load(getFileReader(), getAllocator(), _filePath, _size);
    };
    bx::FileReaderI* getFileReader() { return s_fileReader; }
        
    bx::AllocatorI* getAllocator() {
        if (NULL == g_allocator) {
            g_allocator = getDefaultAllocator();
        }
        return g_allocator;
    };
    void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size) {
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
    bx::AllocatorI* getDefaultAllocator() {
        BX_PRAGMA_DIAGNOSTIC_PUSH();
        BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(
            4459);  // warning C4459: declaration of 's_allocator' hides global declaration
        BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
        static bx::DefaultAllocator s_allocator;
        return &s_allocator;
        BX_PRAGMA_DIAGNOSTIC_POP();
    }
};
}  // namespace components

}  // namespace knot */
