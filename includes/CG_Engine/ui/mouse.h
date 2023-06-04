#ifndef CG_ENGINE_MOUSE_H
#define CG_ENGINE_MOUSE_H

class Mouse {
public:
    static void cursorPosCallback(double _x, double _y) {
        x = _x;
        y = _y;

        if (firstMouse) {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        dx = x - lastX;
        dy = lastY - y; // y coordinates are inverted
        lastX = x;
        lastY = y;
    };

    static void mouseButtonCallback(int button, int action) {
        if (action != sf::Event::MouseButtonReleased) {
            if (!buttons[button]) {
                buttons[button] = true;
            }
        }
        else {
            buttons[button] = false;
        }
        buttonsChanged[button] = action != sf::Event::MouseButtonReleased;
    };

    static void mouseWheelCallback(double _delta) {
        delta = _delta;
    };

    static double getMouseX() {
        return x;
    };

    static double getMouseY() {
        return y;
    };

    static double getDX() {
        double _dx = dx;
        dx = 0;
        return _dx;
    };

    static double getDY() {
        double _dy = dy;
        dy = 0;
        return _dy;
    };

    static double getScrollDY() {
        double _delta = delta;
        delta = 0;
        return _delta;
    };

    static bool button(int button) {
        return buttons[button];
    };

    static bool buttonChanged(int button) {
        bool ret = buttonsChanged[button];
        buttonsChanged[button] = false;
        return ret;
    };

    static bool buttonWentUp(int button) {
        return !buttons[button] && buttonChanged(button);
    };

    static bool buttonWentDown(int button) {
        return buttons[button] && buttonChanged(button);
    };

private:
    static double x;
    static double y;

    static double lastX;
    static double lastY;

    static double dx;
    static double dy;

    static double delta;

    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];
};

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::delta = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[4] = {0};
bool Mouse::buttonsChanged[4] = {0};

#endif //CG_ENGINE_MOUSE_H
