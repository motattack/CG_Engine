#include <CG_Engine/engine.h>

int main() {
    core.init();

    while (core.run()) {
        core.update();
    }

    return 0;
}