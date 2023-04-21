#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>

using namespace std;

int main() {
    sf::Window window(sf::VideoMode(1200, 800), "Hello World");

    window.setActive(true);

    // Check Glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "Error:: glew not init =(: " << glewGetErrorString(glewError) << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        /* Update */

        /* Render */

        glClearColor(0.7f, 0.7f, 7.0f, 0.0f); // 0.0f - 1.0f
        glClear(GL_COLOR_BUFFER_BIT);

        // Display
        window.display();

        // Poll events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return 0;
}