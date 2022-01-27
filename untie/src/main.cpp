#include <knoting/log.h>
#include <knoting/window.h>
#include <iostream>

using namespace knot;

int main() {
    log::Logger::setup();

    Window window(1280, 720, "Knoting");

    while (window.is_open()) {
        window.update();
        window.draw();
    }

    return 0;
}
