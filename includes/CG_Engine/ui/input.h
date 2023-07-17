#ifndef CG_ENGINE_INPUT_H
#define CG_ENGINE_INPUT_H

#include <bitset>
#include <CG_Engine/engine.h>

enum class MouseBtn {
    LEFT = 0, WHEEL = 1, RIGHT = 2, xBUTTON = 3, yBUTTON = 4
}; // xBUTTON и yBUTTON - любые другие

class Input {
private:
    Input() : mouse(0), keyboard(0),
              mousePrevScroll(0.0f), mousePrevPos(0.0f),
              mouseMotionOff(0.0f), mouseScrollOff(0.0f),
              onMouseScroll(false), onMouseMotion(false) {}

    bool onMouseScroll;
    bool onMouseMotion;

    std::bitset<sf::Mouse::ButtonCount> mouse;
    std::bitset<sf::Keyboard::KeyCount> keyboard;

    Vec2 mousePrevScroll;
    Vec2 mousePrevPos;
    Vec2 mouseMotionOff;
    Vec2 mouseScrollOff;
public:
    ~Input() = default;

    static Input &Event() {
        static Input preference;
        return preference;
    }

    void setKey(int key, bool value) {
        if (key != -1)
            keyboard[key] = value;
    }

    const bool IsKeyPressed(int key) {
        return keyboard[key];
    }

    void SetMouseButton(int button, bool value) {
        mouse[button] = value;
    }

    const Vec2 mouseMotionOffset() {
        return mouseMotionOff;
    }

    const Vec2 mouseScrollOffset() {
        return mouseScrollOff;
    }

    const bool isMouseDown(MouseBtn button) {
        return mouse[(int) button];
    }

    void resetScrollState() {
        onMouseScroll = false;
    }

    const bool isMouseMoving() {
        return onMouseMotion;
    }

    const bool isMouseScrolling() {
        return onMouseScroll;
    }

    void mouseScroll(double sx, double sy) {
        mouseScrollOff = Vec2(sx, sy);
        onMouseScroll = true;
    };

    void mouseMotion(double mx, double my) {
        onMouseMotion = false;
        if (mousePrevPos.x != mx || mousePrevPos.y != my) {
            mouseMotionOff.x = mx - mousePrevPos.x;
            mouseMotionOff.y = my - mousePrevPos.y;
            mousePrevPos = Vec2(mx, my);
            onMouseMotion = true;
        }
    };

    float getDX() {
        float dx = mouseMotionOff.x;
        mouseMotionOff.x = 0;
        return dx;
    };

    float getDY() {
        float dy = mouseMotionOff.y;
        mouseMotionOff.y = 0;
        return dy;
    };
};

static void WindowResizedCallback(int width, int height) {
    glViewport(0, 0, width, height);
}

static void MouseButtonCallback(int button, bool pressed) {
    Input::Event().SetMouseButton(button, pressed);
}

static void MouseScrollCallback(double sx, double sy) {
    Input::Event().mouseScroll(sx, sy);
}

static void MouseMoveCallback(double mx, double my) {
    Input::Event().mouseMotion(mx, my);
}

static void KeyboardCallback(sf::Keyboard::Key key, bool pressed) {
    auto &event = Input::Event();

    event.setKey(key, pressed);

    if (event.IsKeyPressed(sf::Keyboard::Escape)) {
        core.exit();
    }

    if (event.IsKeyPressed(sf::Keyboard::Space)) {
        core.stopGame();
    }
}

static void window_close_callback(sf::Window &window) {
    window.close();
}

#endif //CG_ENGINE_INPUT_H
