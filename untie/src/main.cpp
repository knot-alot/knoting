#include "untie.h"

using namespace knot;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage:\n\tknoting [client/server]\n";
    }
    std::string arg = argv[1];
    if (arg == "client") {
        std::cout << "client \n";
    } else if (arg == "server") {
        Engine::isClient = false;
        std::cout << "server \n";
    } else {
        std::cout << "Invalid Argument \n";
        return 0;
    }
    Untie untie;
    untie.run();

    return 0;
}
