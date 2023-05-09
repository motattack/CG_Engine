#include <CG_Engine_Core/object.h>

int main() {
    ObjectManager manager;
    Sphere sphere;
    Cube cube;

    std::cout << cube.test.Position.x << std::endl;

    manager.draw_all();
    return 0;
}