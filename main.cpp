#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
#include "CG_Engine/events.h"
#include "CG_Engine/source.h"

int main() {
    Resource.init();
    core.init();
    timer.init();

    while (core.run()) {
        timer.tick();
        event.poll();

        core.update();
        core.render();
    }

    return 0;
}