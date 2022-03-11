
#pragma once
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/sort.h>
#include <bx/string.h>
#include <bx/timer.h>

#include <fstream>
#include "knoting/font_manager.h"
#include "knoting/text_buffer_manager.h"

#include <wchar.h>

#include <bgfx/bgfx.h>
#include <knoting/components.h>
#include <knoting/engine.h>

namespace knot {
class Window;
}

namespace knot {
class Untie {
   public:
    Untie();

    void run();

   private:
    std::unique_ptr<knot::Engine> m_engine;
    std::unique_ptr<knot::Engine> m_loadEngine;
};

}  // namespace knot
