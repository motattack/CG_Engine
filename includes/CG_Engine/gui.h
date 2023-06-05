#ifndef CG_ENGINE_GUI_H
#define CG_ENGINE_GUI_H

// Gui Variables
// Directional Light
static Vec3 dir_direction = {-0.2f, -1.0f, -0.5f};
static Vec3 dir_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 dir_diffuse = {0.4f, 0.4f, 0.4f};
static Vec3 dir_specular = {0.6f, 0.6f, 0.6f};

// Point Light
//static float point_position
static Vec3 point_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 point_diffuse = {0.5f, 0.5f, 0.5f};
static Vec3 point_specular = {1.0f, 1.0f, 1.0f};
static float point_constant = 1.0f;
static float point_linear = 0.09f;
static float point_quadratic = 0.032f;

// Spot Light
static Vec3 spot_ambient = {0.2f, 0.2f, 0.2f};
static Vec3 spot_diffuse = {1.0f, 1.0f, 1.0f};
static Vec3 spot_specular = {1.0f, 1.0f, 1.0f};
static float spot_constant = 1.0f;
static float spot_linear = 0.09f;
static float spot_quadratic = 0.032f;
static float spot_cutOff = 12.5f;
static float spot_outerCutOff = 18.5f;

static float cubeMapPos[3] = {0.0f, 0.0f, 0.0f};

void light_editor(Scene &scene, sf::Clock dt) {
    ImGui::SFML::Update(scene.window, dt.restart());

    float FPS = ImGui::GetIO().Framerate;
    ImGui::Begin("Hello, world!");
    ImGui::Text("FPS = %f", FPS);
    ImGui::Text("position v(%f, %f, %f)", scene.camera.Position.x, scene.camera.Position.y,
                scene.camera.Position.z);

    ImGuiStyle *style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
    ImGui::PopStyleColor();

    ImGui::SliderFloat3("Map Position", cubeMapPos, -10.0f, 10.0f);
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

    ImGui::SFML::Render(scene.window);

    glEnable(GL_DEPTH_TEST);
}

#endif //CG_ENGINE_GUI_H
