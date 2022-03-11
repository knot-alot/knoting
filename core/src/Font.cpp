#include <knoting/Font.h>

namespace font {
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
void* load(const char* _filePath, uint32_t* _size) {
    return load(entry::getFileReader(), entry::getAllocator(), _filePath, _size);
};
}  // namespace font

