#ifndef CG_ENGINE_MOUSE_H
#define CG_ENGINE_MOUSE_H

class Mouse {
public:
    static void cursorPosCallback(float _x, float _y) {
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

    static void mouseWheelCallback(float _delta) {
        delta = _delta;
    };

    static float getMouseX() {
        return x;
    };

    static float getMouseY() {
        return y;
    };

    static float getDX() {
        float _dx = dx;
        dx = 0;
        return _dx;
    };

    static float getDY() {
        float _dy = dy;
        dy = 0;
        return _dy;
    };

    static float getScrollDY() {
        float _delta = delta;
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
    static float x;
    static float y;

    static float lastX;
    static float lastY;

    static float dx;
    static float dy;

    static float delta;

    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];
};

float Mouse::x = 0;
float Mouse::y = 0;

float Mouse::lastX = 0;
float Mouse::lastY = 0;

float Mouse::dx = 0;
float Mouse::dy = 0;

float Mouse::delta = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[4] = {0};
bool Mouse::buttonsChanged[4] = {0};

#endif //CG_ENGINE_MOUSE_H
