#ifndef CG_ENGINE_KEYBOARD_H
#define CG_ENGINE_KEYBOARD_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class Keyboard {
public:
    static void keyCallback(sf::Event &event) {
        if (event.type == sf::Event::KeyPressed) {
            Keyboard::keys[event.key.code] = true;
            Keyboard::keysChanged[event.key.code] = true;
        } else if (event.type == sf::Event::KeyReleased) {
            Keyboard::keys[event.key.code] = false;
            Keyboard::keysChanged[event.key.code] = true;
        }
    }

    static bool key(int key) {
        return keys[key];
    };

    static bool keyChanged(int key) {
        bool ret = keysChanged[key];
        keysChanged[key] = false;
        return ret;
    };

    static bool keyWentUp(int key) {
        return keys[key] && keyChanged(key);
    };

    static bool keyWentDown(int key) {
        return !keys[key] && keyChanged(key);
    };

private:
    static bool keys[];
    static bool keysChanged[];
};

bool Keyboard::keys[sf::Keyboard::KeyCount] = {false};
bool Keyboard::keysChanged[sf::Keyboard::KeyCount] = {false};

#endif //CG_ENGINE_KEYBOARD_H
