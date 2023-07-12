#include <CG_Engine/engine.h>
#include <CG_Engine/timer/timer.h>
#include <CG_Engine/events/events.h>
#include <CG_Engine/ecs/base/types.h>
#include "CG_Engine/ecs/base/EntityManager.h"
#include "CG_Engine/ecs/base/Entity.h"

struct TestComp1 : public BaseComponent {
    int A = 5;
};

struct TestComp2 : public BaseComponent {
    int A = 5;
};


struct TestSystem1 : public BaseSystem {
    TestSystem1() {
        addComponentSignature<TestComp1>();
    }
};

struct TestSystem2 : public BaseSystem {
    TestSystem2() {
        addComponentSignature<TestComp2>();
    }
};

struct TestSystem3 : public BaseSystem {
    TestSystem3() {
        addComponentSignature<TestComp1>();
        addComponentSignature<TestComp2>();
    }
};

int main() {
    EntityManager mgr;

    mgr.registerSystem<TestSystem1>();
    mgr.registerSystem<TestSystem2>();
    mgr.registerSystem<TestSystem3>();

    auto entity1 = mgr.addNewEntity();
    Entity ent(entity1, &mgr);
    ent.addComponent<TestComp1>();

    auto entity2 = mgr.addNewEntity();
    mgr.addComponent<TestComp2>(entity2);

    auto entity3 = mgr.addNewEntity();
    mgr.addComponent<TestComp1>(entity3);
    mgr.addComponent<TestComp2>(entity3);

    mgr.update();

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