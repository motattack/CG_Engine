#include <CG_Engine/engine.h>
#include <CG_Engine/timer/timer.h>
#include <CG_Engine/events/events.h>
#include <CG_Engine/ecs/base/types.h>

struct Component {
public:
    virtual ~Component(){};
};

class TestComp1 : public Component {

};

class TestComp2 : public Component {

};

int main() {
    auto typeID1 = CompType<TestComp1>();
    auto typeID2 = CompType<TestComp1>();

    auto typeID3 = CompType<TestComp2>();

    std::cout << typeID1 << typeID2 << typeID3;

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