#ifndef CG_ENGINE_GUI_H
#define CG_ENGINE_GUI_H

#include <imgui.h>
#include <imgui-SFML.h>
#include <CG_Engine/timer.h>
#include <CG_Engine/engine.h>
#include <sstream>
#include <fstream>
#include <CG_Engine/components/name.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include "CG_Engine/objects/model.h"
#include "CG_Engine/components/modelRenderer.h"
#include "direct.h"
#include "CG_Engine/components/meshRenderer.h"
#include "CG_Engine/components/spotLight.h"
#include "CG_Engine/components/directionalLight.h"
#include "CG_Engine/components/pointLight.h"


static int selectedEntityIndex = 0;

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

// Function to handle button-like combo box
bool ComboButton(const char *label, const char **items, int *current_item, int items_count) {
    ImVec2 button_size(150, 30);

    if (ImGui::BeginCombo(label, items[*current_item], 0)) {
        for (int i = 0; i < items_count; ++i) {
            bool is_selected = (*current_item == i);
            if (ImGui::Selectable(items[i], is_selected))
                *current_item = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus(); // Set the selected item as the default focus
        }
        ImGui::EndCombo();
    }

    // Check if the combo box is closed, meaning the user clicked on the button part
    return !ImGui::IsPopupOpen(label);
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
            MakeOrDelete();
            MyImGuiWindow();
        }

        ImGui::SFML::Render(core.Window());
    }

    void info() {
        ImGui::Begin("Info");
        if (selectedEntityIndex != -1) {
            auto &name = Manager.getComponent<EntityName>(selectedEntityIndex);
            ImGui::Text("Selected Entity:");
            ImGui::Text("Name: %s", name.Value.c_str());
            ImGui::Text("ID: %d", name.getId());
        }
        ImGui::End();
    }

    void addEntity() {
        EntityId newEntity = Manager.addNewEntity();
        Manager.addComponent<Transform>(newEntity);
        Manager.addComponent<EntityName>(newEntity, "Empty Entity");
    }

    void MyImGuiWindow() {
        ImGui::Begin("Add component");
        const char *items[] = {"Camera", "Model", "Mesh", "Direct", "Spot", "Point"};
        static int selected_item = 0;

        if (selectedEntityIndex != -1)
            if (ComboButton("Select", items, &selected_item, IM_ARRAYSIZE(items))) {
                switch (selected_item) {
                    case 0: // Option 1
                        if (ImGui::Button("add")) {
                            Manager.addComponent<Camera>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<Camera>(selectedEntityIndex);
                        }
                        break;
                    case 1:
                        if (ImGui::Button("add")) {
                            Manager.addComponent<ModelRenderer>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<ModelRenderer>(selectedEntityIndex);
                        }
                        break;
                    case 2:
                        if (ImGui::Button("add")) {
                            Manager.addComponent<MeshRenderer>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<MeshRenderer>(selectedEntityIndex);
                        }
                        break;
                    case 3:
                        if (ImGui::Button("add")) {
                            Manager.addComponent<DirectionalLight>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<DirectionalLight>(selectedEntityIndex);
                        }
                        break;
                    case 4:
                        if (ImGui::Button("add")) {
                            Manager.addComponent<SpotLighting>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<SpotLighting>(selectedEntityIndex);
                        }
                        break;
                    case 5:
                        if (ImGui::Button("add")) {
                            Manager.addComponent<PointLighting>(selectedEntityIndex);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("remove")) {
                            Manager.removeComponent<PointLighting>(selectedEntityIndex);
                        }
                        break;
                }
            }
        ImGui::End();
    }


    void entityList() {
        ImGui::Begin("Entity list");

        for (const auto &entity: Manager.ActiveEntities()) {
            auto &name = Manager.getComponent<EntityName>(entity);
            std::string entityLabel = name.Value + " (ID: " + std::to_string(name.getId()) + ")";
            bool isSelected = (selectedEntityIndex == entity);
            if (ImGui::Selectable(entityLabel.c_str(), isSelected))
                selectedEntityIndex = entity;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::End();

        info();
    }

    void MakeOrDelete() {
        ImGui::Begin("Action", nullptr, ImGuiDockNodeFlags_AutoHideTabBar);
        if (ImGui::Button("+")) {
            addEntity();
        }
        if (selectedEntityIndex != -1) {
            ImGui::SameLine();
            if (ImGui::Button("-")) {
                Manager.destroyEntity(selectedEntityIndex);
                selectedEntityIndex = -1;
            }
        }
        ImGui::End();
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
            if (selectedEntityIndex != -1) {
                camera();
                transform();
                model();
                mesh();
            }
            ImGui::End();
        }
    }

    void transform() {
        if (Manager.hasComponent<Transform>(selectedEntityIndex)) {
            auto &instance = Manager.getComponent<Transform>(selectedEntityIndex);
            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::Indent();

                if (ImGui::CollapsingHeader("Translation")) {
                    ImGui::InputFloat("X", &instance.Position.x, 0.05f, 1.0f);
                    ImGui::InputFloat("Y", &instance.Position.y, 0.05f, 1.0f);
                    ImGui::InputFloat("Z", &instance.Position.z, 0.05f, 1.0f);
                }
                if (ImGui::CollapsingHeader("Rotation")) {
                    ImGui::SliderFloat3("Angles", &instance.Rotation.x, -180.0f, 180.0f);
                    if (ImGui::Button("Reset Rotation")) {
                        instance.Rotation = Vec3(0.f, 0.f, 0.f);
                    }
                }

                if (ImGui::CollapsingHeader("Scaling")) {
                    ImGui::InputFloat("Factor", &instance.Scale, 0.05f, 1.0f);
                    if (ImGui::Button("Reset Scale")) {
                        instance.Scale = 1.f;
                    }
                }
                ImGui::Unindent();
            }
        }
    }

    void camera() {
        if (Manager.hasComponent<Camera>(selectedEntityIndex)) {
            auto &instance = Manager.getComponent<Camera>(selectedEntityIndex);

            if (ImGui::CollapsingHeader("Camera")) {
                ImGui::Text("Yaw: %.2f", instance.Yaw);
                ImGui::SameLine();
                ImGui::Text("Pitch: %.2f", instance.Pitch);

                ImGui::SliderFloat("Sensitivity", &instance.Sensitivity, 0.1f, 10.0f);
                ImGui::InputFloat("MovementSpeed", &instance.MovementSpeed, 0.05f, 5.0f);

                ImGui::InputFloat("FOV", &instance.Fov, 1.f, 180.0f);
                ImGui::InputFloat("zFar", &instance.zFar, 0.05f, 5.0f);
                ImGui::InputFloat("zNear", &instance.zNear, 0.05f, 5.0f);
                ImGui::SliderFloat("AspectRatio", &instance.AspectRatio, 4.0f / 3.0f, 21.0f / 9.0f);
            }
        }
    }

    void model() {
        if (Manager.hasComponent<ModelRenderer>(selectedEntityIndex)) {
            auto &instance = Manager.getComponent<ModelRenderer>(selectedEntityIndex);

            if (ImGui::CollapsingHeader("Model")) {
                ImGui::Indent();

                if (ImGui::CollapsingHeader("Loaded"))
                    modelSelector(instance);
                if (ImGui::CollapsingHeader("Load New"))
                    modelLoader();

                ImGui::Unindent();
            }

        }
    }

    void modelSelector(auto &instance) {
        static int selectedModelIndex = -1;
        int index = 0;
        for (const auto &modelPair: Resource.getModels()) {
            const std::string &modelName = modelPair.first;
            bool isSelected = (index == selectedModelIndex);
            if (ImGui::Selectable(modelName.c_str(), isSelected))
                selectedModelIndex = index;

            if (isSelected)
                ImGui::SetItemDefaultFocus();

            index++;
        }

        if (selectedModelIndex != -1)
            if (ImGui::Button("Set")) {
                auto it = Resource.getModels().begin();
                std::advance(it, selectedModelIndex);
                const std::string &selectedModelName = it->first;
                instance.Renderer = Resource.getModel(selectedModelName);
            }
    }

    void modelLoader() {
        static char name[256] = "";
        ImGui::InputText("Model name", name, sizeof(name));

        static char path[256] = "";
        ImGui::InputText("Model path", path, sizeof(path));

        if (ImGui::Button("Load")) {
            std::cout << name << path << std::endl;
            Resource.loadModel(name, path);
        }
    }

    void mesh() {
        if (Manager.hasComponent<MeshRenderer>(selectedEntityIndex)) {
            auto &instance = Manager.getComponent<MeshRenderer>(selectedEntityIndex);

            if (ImGui::CollapsingHeader("Mesh")) {
                ImGui::Text("Mesh: # %d %s", instance.getId(), instance.Name.c_str());
            }
            static char filePath[256] = ""; // Store the file path here
            ImGui::InputText("File Path", filePath, sizeof(filePath));
            if (ImGui::Button("Load")) {
                std::cout << filePath << std::endl;
                instance.Mesh = Resource.getMesh("CUBE");
            }
        }
    }

private:
    int selected;
    std::vector<int> objects;
};

static GUI &gui = GUI::Get();
#endif //CG_ENGINE_GUI_H
