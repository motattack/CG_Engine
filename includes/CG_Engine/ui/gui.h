#ifndef CG_ENGINE_GUI_H
#define CG_ENGINE_GUI_H

#include <imgui.h>
#include <imgui-SFML.h>
#include <CG_Engine/timer.h>
#include <CG_Engine/engine.h>
#include <sstream>
#include <fstream>
#include "CG_Engine/components/name.h"

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
    ss << std::hex << hexColor.substr(1);
    ss >> hexValue;
    color.x = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;
    color.y = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;
    color.z = static_cast<float>(hexValue & 0xFF) / 255.0f;
    color.w = 1.0f;
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
}

class GUI {
public:
    static GUI &Get() {
        static GUI reference;
        return reference;
    }

    void init() {
        ImGuiIO &IO = ImGui::GetIO();
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // https://github.com/SFML/imgui-sfml/issues/72 :d
        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        loadImguiStyleFromFile("res/gui.txt");
    }

    void display() {
        sf::Time dt = sf::seconds(timer.deltaTime());

        ImGui::SFML::Update(core.Window(), dt);

        {
            ImGui::ShowDemoWindow();
            space();
            components();
            entityList();
        }

//        ImGui::Begin("Tool");
//        if (ImGui::Button("Play")) {
//            core.mode = Mode::GAME;
//        }
//        ImGui::End();


        ImGui::SFML::Render(core.Window());
    }

    void entityList(){
        // Prepare a list of entity names and IDs
        std::vector<std::string> entityNames;
        std::vector<EntityId> entityIDs; // Assuming you have a way to get the entity IDs
        int indexToRemove = -1; // To track the index to be removed

        for (auto& entity : Manager.ActiveEntities()) {
            // Assuming `EntityName` is a string type component
            auto& name = Manager.getComponent<EntityName>(entity);
            entityNames.push_back(name.Value);
            entityIDs.push_back(entity);
        }

        // Convert the list of entity names to a C-style array for ImGui
        const char** entityNamesArr = new const char*[entityNames.size()];
        for (size_t i = 0; i < entityNames.size(); ++i) {
            entityNamesArr[i] = entityNames[i].c_str();
        }

        // ImGui code to create a combo box
        static int selectedEntityIndex = 0; // To store the selected entity index
        if (ImGui::BeginCombo("Entities", entityNamesArr[selectedEntityIndex])) {
            for (int i = 0; i < entityNames.size(); ++i) {
                bool isSelected = (selectedEntityIndex == i);
                if (ImGui::Selectable(entityNamesArr[i], isSelected)) {
                    selectedEntityIndex = i;
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus(); // Set the default selection
                }
            }
            ImGui::EndCombo();
        }

        // ImGui code to add "Delete" button
        if (selectedEntityIndex != -1) {
            if (ImGui::Button("Delete")) {
                indexToRemove = selectedEntityIndex;
            }
        }

        // Handle entity removal after ImGui frame ends
        if (indexToRemove != -1) {
            std::cout << "Removing entity at index " << indexToRemove << ", ID: " << entityIDs[indexToRemove] << std::endl;
            // Code here to remove the entity with the ID entityIDs[indexToRemove]
            // ...
            // Reset indexToRemove to -1 to avoid accidental removal
            indexToRemove = -1;
        }

        // Clean up C-style array
        delete[] entityNamesArr;
    }

    void space() {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;


        ImGui::Begin("space", nullptr, window_flags);
        {
            ImGuiID id = ImGui::GetID("space");
            ImGui::DockSpace(id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        }

        menu();

        ImGui::End();
        ImGui::PopStyleVar(3);
    }

    void menu() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    std::cout << "maybe add later" << std::endl;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("View Help", "Ctrl+H")) {
                    std::cout << "CG_Engine 2023.07" << std::endl;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void components() {
        ImGui::Begin("Components");
        {
            ImGui::Begin("Toolbar");
            {
                if (ImGui::Button("add")) {

                }

                ImGui::SameLine();
                if (ImGui::Button("remove")) {

                }
                ImGui::End();
            }

            if (selected > -1) {
            }
            ImGui::End();
        }
    }

private:
    int selected;
    std::vector<int> objects;
};

static GUI &gui = GUI::Get();
#endif //CG_ENGINE_GUI_H
