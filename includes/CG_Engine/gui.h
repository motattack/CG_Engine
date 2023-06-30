#ifndef CG_ENGINE_GUI_H
#define CG_ENGINE_GUI_H

// Gui Variables
// Directional Light
static Vec3 dir_direction = {-0.2f, -1.0f, -0.5f};
static Vec3 dir_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 dir_diffuse = {0.4f, 0.4f, 0.4f};
static Vec3 dir_specular = {0.5f, 0.5f, 0.5f};

// Point Light
//static float point_position
static Vec3 point_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 point_diffuse = {0.5f, 0.5f, 0.5f};
static Vec3 point_specular = {1.0f, 1.0f, 1.0f};
static float point_constant = 1.0f;
static float point_linear = 0.09f;
static float point_quadratic = 0.032f;

// Spotlight
static Vec3 spot_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 spot_diffuse = {1.0f, 1.0f, 1.0f};
static Vec3 spot_specular = {1.0f, 1.0f, 1.0f};
static float spot_constant = 1.0f;
static float spot_linear = 0.09f;
static float spot_quadratic = 0.032f;
static float spot_cutOff = 12.5f;
static float spot_outerCutOff = 18.5f;

void light_editor(Scene &scene) {
    float FPS = ImGui::GetIO().Framerate;
    ImGui::Begin("Hello, world!");
    ImGui::Text("FPS = %f", FPS);
    ImGui::Text("position v(%f, %f, %f)", scene.camera.Position.x, scene.camera.Position.y,
                scene.camera.Position.z);

    ImGuiStyle *style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
    ImGui::PopStyleColor();

    ImGui::Text("Directional Light");
    ImGui::SliderFloat3("Directional Direction", &dir_direction[0], -2.0f, 2.0f);
    ImGui::SliderFloat3("Directional Ambient", &dir_ambient[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Directional Diffuse", &dir_diffuse[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Directional Specular", &dir_specular[0], 0.0f, 1.0f);
    ImGui::Text("Point Light");
    ImGui::SliderFloat3("Point Ambient", &point_ambient[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Point Diffuse", &point_diffuse[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Point Specular", &point_specular[0], 0.0f, 1.0f);
    ImGui::SliderFloat("Point Constant", &point_constant, -1.0f, 1.0f);
    ImGui::SliderFloat("Point Linear", &point_linear, -1.0f, 1.0f);
    ImGui::SliderFloat("Point Quadratic", &point_quadratic, -1.0f, 1.0f);
    ImGui::Text("Spot Light");
    ImGui::SliderFloat3("Spot Ambient", &spot_ambient[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Spot Diffuse", &spot_diffuse[0], 0.0f, 1.0f);
    ImGui::SliderFloat3("Spot Specular", &spot_specular[0], 0.0f, 1.0f);
    ImGui::SliderFloat("Spot Constant", &spot_constant, -1.0f, 1.0f);
    ImGui::SliderFloat("Spot Linear", &spot_linear, -1.0f, 1.0f);
    ImGui::SliderFloat("Spot Quadratic", &spot_quadratic, -1.0f, 1.0f);
    ImGui::SliderFloat("Spot CutOff", &spot_cutOff, 0.0f, 100.0f);
    ImGui::SliderFloat("Spot OuterCutOff", &spot_outerCutOff, 0.0f, 100.0f);
    ImGui::End();
}

void ModelsManager(Manager &manager) {
    static int selectedModel = -1;
    ImGui::Begin("Models");

//    static bool my_tool_active = true;
//    ImGui::Begin("Models", &my_tool_active, ImGuiWindowFlags_MenuBar);
//
//    if (ImGui::BeginMenuBar()) {
//        if (ImGui::BeginMenu("File")) {
//            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
//            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
//            if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
//            ImGui::EndMenu();
//        }
//        ImGui::EndMenuBar();
//    }

    const char **listbox_items = manager.getBaseName();

    ImGui::ListBox("Base", &selectedModel, listbox_items, manager.bSize());

    if (selectedModel >= 0 && selectedModel < manager.bSize()) {
        static char str[128] = "Sample Text";
        ImGui::InputText("Name", str, IM_ARRAYSIZE(str));
        if (ImGui::Button("Add Model")) {
            Object newObject = manager.getBaseByID(selectedModel);
            newObject.name = str;
            manager.addObject(newObject);
        }
    }

    ImGui::End();
}

static int selectedInstance = -1;

void ObjectManager(Manager &manager) {


    // Draw a list of instances
    ImGui::Begin("Layers");

    const char **listbox_items = manager.getName();

    ImGui::ListBox("Instances", &selectedInstance, listbox_items, manager.size());

    ImGui::End();

    if (selectedInstance >= 0 && selectedInstance < manager.size()) {
        Object &instance = manager.getByID(selectedInstance);

        ImGui::Begin("Instance Properties");
        if (ImGui::CollapsingHeader("Translate")) {
            ImGui::InputFloat("X", &instance.pos.x, 0.05f, 1.0f);
            ImGui::InputFloat("Y", &instance.pos.y, 0.05f, 1.0f);
            ImGui::InputFloat("Z", &instance.pos.z, 0.05f, 1.0f);
        }
        if (ImGui::CollapsingHeader("Rotate")) {
            ImGui::SliderFloat3("angles", &instance.rotation.x, -180.0f, 180.0f);
            if (ImGui::Button("Reset Rotate")) {
                instance.rotation = Vec3(0.f, 0.f, 0.f);
            }
        }

        if (ImGui::CollapsingHeader("Scale")) {
            ImGui::InputFloat("Factor", &instance.scale, 0.05f, 1.0f);
            if (ImGui::Button("Reset Scale")) {
                instance.scale = 1.f;
            }
        }

        ImGui::End();
    }
}

static int selectedType = -1;

void WidgetMaterial(Manager &manager) {
    const char *list_types[] = {"texture_diffuse", "texture_specular"};
    static char str[128] = "res/textures/box.png";

    if (selectedInstance >= 0 && selectedInstance < manager.size()) {
        ImGui::Begin("Material");
        if (ImGui::CollapsingHeader("Path to texture")) {

            ImGui::ListBox("Type", &selectedType, list_types, 2);

            if (selectedType >= 0) {
                ImGui::InputText("Path", str, IM_ARRAYSIZE(str));

                if (ImGui::Button("Change texture")) {
                    Object &curObject = manager.getByID(selectedInstance);
                    curObject.changeTexture(str, list_types[selectedType]);
                }
            }
        }

        ImGui::End();
    }
}

#endif //CG_ENGINE_GUI_H
