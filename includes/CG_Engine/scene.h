#ifndef CG_ENGINE_SCENE_H
#define CG_ENGINE_SCENE_H

#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/ui/camera.h>
#include <CG_Engine/math/vec3.h>
#include <CG_Engine/render/light.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <CG_Engine/ui/mouse.h>
#include <CG_Engine/ui/keyboard.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "objectManager.h"

#include <unordered_map>


class Scene {
public:
    Scene() = default;

    Scene(const char *title, unsigned int scrWidth, unsigned int scrHeight) : title(title) {
        this->scrWidth = scrWidth;
        this->scrHeight = scrHeight;

        setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
    };


    void setWindowColor(float r, float g, float b, float a) {
        bg[0] = r;
        bg[1] = g;
        bg[2] = b;
        bg[3] = a;
    };

    std::unordered_map<std::string, ImGuiCol_> colorMap = {
            {"Text",                  ImGuiCol_Text},
            {"TextDisabled",          ImGuiCol_TextDisabled},
            {"WindowBg",              ImGuiCol_WindowBg},
            {"TextDisabled",          ImGuiCol_TextDisabled},
            {"WindowBg",              ImGuiCol_WindowBg},
            {"ChildBg",               ImGuiCol_ChildBg},
            {"PopupBg",               ImGuiCol_PopupBg},
            {"Border",                ImGuiCol_Border},
            {"BorderShadow",          ImGuiCol_BorderShadow},
            {"FrameBg",               ImGuiCol_FrameBg},
            {"FrameBgHovered",        ImGuiCol_FrameBgHovered},
            {"FrameBgActive",         ImGuiCol_FrameBgActive},
            {"TitleBg",               ImGuiCol_TitleBg},
            {"TitleBgActive",         ImGuiCol_TitleBgActive},
            {"TitleBgCollapsed",      ImGuiCol_TitleBgCollapsed},
            {"MenuBarBg",             ImGuiCol_MenuBarBg},
            {"ScrollbarBg",           ImGuiCol_ScrollbarBg},
            {"ScrollbarGrab",         ImGuiCol_ScrollbarGrab},
            {"ScrollbarGrabHovered",  ImGuiCol_ScrollbarGrabHovered},
            {"ScrollbarGrabActive",   ImGuiCol_ScrollbarGrabActive},
            {"CheckMark",             ImGuiCol_CheckMark},
            {"SliderGrab",            ImGuiCol_SliderGrab},
            {"SliderGrabActive",      ImGuiCol_SliderGrabActive},
            {"Button",                ImGuiCol_Button},
            {"ButtonHovered",         ImGuiCol_ButtonHovered},
            {"ButtonActive",          ImGuiCol_ButtonActive},
            {"Header",                ImGuiCol_Header},
            {"HeaderHovered",         ImGuiCol_HeaderHovered},
            {"HeaderActive",          ImGuiCol_HeaderActive},
            {"Separator",             ImGuiCol_Separator},
            {"SeparatorHovered",      ImGuiCol_SeparatorHovered},
            {"SeparatorActive",       ImGuiCol_SeparatorActive},
            {"ResizeGrip",            ImGuiCol_ResizeGrip},
            {"ResizeGripHovered",     ImGuiCol_ResizeGripHovered},
            {"ResizeGripActive",      ImGuiCol_ResizeGripActive},
            {"Tab",                   ImGuiCol_Tab},
            {"TabHovered",            ImGuiCol_TabHovered},
            {"TabActive",             ImGuiCol_TabActive},
            {"TabUnfocused",          ImGuiCol_TabUnfocused},
            {"TabUnfocusedActive",    ImGuiCol_TabUnfocusedActive},
            {"DockingPreview",        ImGuiCol_DockingPreview},
            {"DockingEmptyBg",        ImGuiCol_DockingEmptyBg},
            {"PlotLines",             ImGuiCol_PlotLines},
            {"PlotLinesHovered",      ImGuiCol_PlotLinesHovered},
            {"PlotHistogram",         ImGuiCol_PlotHistogram},
            {"PlotHistogramHovered",  ImGuiCol_PlotHistogramHovered},
            {"TableHeaderBg",         ImGuiCol_TableHeaderBg},
            {"TableBorderStrong",     ImGuiCol_TableBorderStrong},
            {"TableBorderLight",      ImGuiCol_TableBorderLight},
            {"TableRowBg",            ImGuiCol_TableRowBg},
            {"TableRowBgAlt",         ImGuiCol_TableRowBgAlt},
            {"TextSelectedBg",        ImGuiCol_TextSelectedBg},
            {"DragDropTarget",        ImGuiCol_DragDropTarget},
            {"NavHighlight",          ImGuiCol_NavHighlight},
            {"NavWindowingHighlight", ImGuiCol_NavWindowingHighlight},
            {"NavWindowingDimBg",     ImGuiCol_NavWindowingDimBg},
            {"ModalWindowDimBg",      ImGuiCol_ModalWindowDimBg},
            {"COUNT",                 ImGuiCol_COUNT}
    };

    ImVec4 HexToImVec4(const std::string &hexColor) {
        ImVec4 color;
        unsigned int hexValue;
        std::stringstream ss;
        ss << std::hex << hexColor.substr(1);  // Exclude the "#" character
        ss >> hexValue;
        color.x = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;
        color.y = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;
        color.z = static_cast<float>(hexValue & 0xFF) / 255.0f;
        color.w = 1.0f;
//        std::cout << color.x << ", " << color.y << ", " << color.z << ", " << color.w << std::endl;
        return color;
    }

    void loadImguiStyleFromFile(const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "error";
            return;
        }

        std::string line;
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string colorName, hexValue;
            if (iss >> colorName >> hexValue) {
                auto it = colorMap.find(colorName);
                if (it != colorMap.end()) {
                    colors[it->second] = HexToImVec4(hexValue);
                }
            }
        }

        file.close();

        style.ChildRounding = 4.0f;
        style.FrameBorderSize = 1.0f;
        style.FrameRounding = 2.0f;
        style.GrabMinSize = 7.0f;
        style.PopupRounding = 2.0f;
        style.ScrollbarRounding = 12.0f;
        style.ScrollbarSize = 13.0f;
        style.TabBorderSize = 1.0f;
        style.TabRounding = 0.0f;
        style.WindowRounding = 4.0f;
    }

    bool init() {
        window.create(sf::VideoMode(scrWidth, scrHeight), title, sf::Style::Titlebar | sf::Style::Close,
                      settings);

        window.setFramerateLimit(75);
        window.setVerticalSyncEnabled(true);
        window.setActive(true);

        ImGui::SFML::Init(window, true);


        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        //setImguiStyle();
        loadImguiStyleFromFile("res/gui.txt");

        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            std::cout << "Error:: glew not init =(" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_CULL_FACE);
//        glFrontFace(GL_CCW);
        setPolygonMode();

        stbi_set_flip_vertically_on_load(true);

        return true;
    };

    bool shouldClose() {
        return !window.isOpen(); // Return true if the SFML window is closed
    }

    void setShouldClose() {
        window.close();
    };

    void newFrame() {
        window.display();
    }

    void cleanup() {
        ImGui::SFML::Shutdown();
        window.close();
    };

    bool pollEvent(sf::Event &event) {
        return window.pollEvent(event);
    }

    void setPolygonMode() const {
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void processInput(float dt) {
        if (not freeMouseMode) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            if (Keyboard::key(sf::Keyboard::W)) {
                camera.Move(CameraDirection::FORWARD, dt);
            }
            if (Keyboard::key(sf::Keyboard::S)) {
                camera.Move(CameraDirection::BACKWARD, dt);
            }
            if (Keyboard::key(sf::Keyboard::D)) {
                camera.Move(CameraDirection::RIGHT, dt);
            }
            if (Keyboard::key(sf::Keyboard::A)) {
                camera.Move(CameraDirection::LEFT, dt);
            }
            if (Keyboard::key(sf::Keyboard::Space)) {
                camera.Move(CameraDirection::UP, dt);
            }
            if (Keyboard::key(sf::Keyboard::LShift)) {
                camera.Move(CameraDirection::DOWN, dt);
            }

            if (Keyboard::key(sf::Keyboard::Left)) {
                camera.Rotate(-dt * 360.f, 0);
            }

            if (Keyboard::key(sf::Keyboard::Right)) {
                camera.Rotate(dt * 360.f, 0);
            }

            if (Keyboard::key(sf::Keyboard::Up)) {
                camera.Rotate(0, dt * 360.f);
            }

            if (Keyboard::key(sf::Keyboard::Down)) {
                camera.Rotate(0, -dt * 360.f);
            }

            if (Keyboard::keyWentUp(sf::Keyboard::L)) {
                torch = !torch;
            }

            camera.Rotate(Mouse::getDX(), Mouse::getDY());

            camera.ChangeFOV(Mouse::getScrollDY());
        }

        if (Keyboard::key(sf::Keyboard::Escape)) {
            setShouldClose();
        }

        if (Keyboard::keyWentUp(sf::Keyboard::E)) {
            wireframeMode = !wireframeMode;
            setPolygonMode();
        }

        if (Keyboard::keyWentUp(sf::Keyboard::M)) {
            freeMouseMode = !freeMouseMode;
        }
    }

    bool getTorch() const {
        return torch;
    }

    void PollEvents() {
        sf::Event event{};
        while (pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            Keyboard::keyCallback(event);
            switch (event.type) {
                case sf::Event::Closed:
                    cleanup();
                    break;
                case sf::Event::MouseMoved:
                    Mouse::cursorPosCallback(static_cast<float>(event.mouseMove.x),
                                             static_cast<float>(event.mouseMove.y));
                    break;
                case sf::Event::MouseWheelScrolled:
                    Mouse::mouseWheelCallback(event.mouseWheelScroll.delta);
                    break;
                default:
                    break;
            }
        }
    }

    sf::RenderWindow window;
    Camera camera;

    Manager manager{};
protected:

    sf::ContextSettings settings{24, 8, 0, 3, 3, sf::ContextSettings::Default};

    const char *title{};
    unsigned int scrWidth = 800;
    unsigned int scrHeight = 600;
    bool torch = false;
    bool wireframeMode = false;
    bool freeMouseMode = true;

    float bg[4]{}; // background color
};

#endif //CG_ENGINE_SCENE_H
