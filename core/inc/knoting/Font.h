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


namespace font {
void* load(const char* _filePath, uint32_t* _size);
void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);

}  // namespace font

  // namespace knot
