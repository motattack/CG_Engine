#ifndef CG_ENGINE_MOUSE_H
#define CG_ENGINE_MOUSE_H

// T

class Mouse {
public:
    static void cursorPosCallback(sf::RenderWindow &window, double _x, double _y) {
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

    static void mouseButtonCallback(sf::RenderWindow &window, int button, int action, int mods) {
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

    static void mouseWheelCallback(sf::RenderWindow &window, double dx, double dy) {
        scrollDx = dx;
        scrollDy = dy;
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

    static double getScrollDX() {
        double _scrollDx = scrollDx;
        scrollDx = 0;
        return _scrollDx;
    };

    static double getScrollDY() {
        double _scrollDy = scrollDy;
        scrollDy = 0;
        return _scrollDy;
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

    static double scrollDx;
    static double scrollDy;

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

double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[sf::Mouse::ButtonCount] = {0};
bool Mouse::buttonsChanged[sf::Mouse::ButtonCount] = {0};

#endif //CG_ENGINE_MOUSE_H
