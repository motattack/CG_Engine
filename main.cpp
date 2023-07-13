#include <CG_Engine/engine.h>
#include <CG_Engine/events.h>


int main() {

    core.init();
    event.init();

    while (core.run()) {
        event.poll();
        core.update();
    }

    return 0;
}