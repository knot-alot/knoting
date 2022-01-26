#include <iostream>

#ifdef KNOTING_RUNTIME

#include <knoting/log.h>
#include <knoting/window.h>

using namespace knot;

int main() {
    log::info("Hi there");
    Window window(1280, 720, "Knoting");

    while (window.is_open()) {
        window.update();
        window.draw();
    }

    return 0;
}

#endif
