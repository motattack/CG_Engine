#include <CG_Engine/engine.h>
#include <CG_Engine/timer/timer.h>
#include <CG_Engine/events/events.h>
#include <CG_Engine/ecs/base/types.h>
#include "CG_Engine/ecs/base/EntityManager.h"

int main() {
    EntityManager mgr;
    auto id = mgr.addNewEntity();
    auto id2 = mgr.addNewEntity();
    auto id3 = mgr.addNewEntity();

    std::cout << id << std::endl;
    std::cout << id2 << std::endl;
    std::cout << id3 << std::endl;

    core.init();
    timer.init();
    event.init();

    while (core.run()) {
        timer.tick();
        event.poll();
        core.update();
    }

    return 0;
}