#ifndef CG_ENGINE_GUI_H
#define CG_ENGINE_GUI_H

#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <CG_Engine_Core/phy/rigidbody.h>

void test(std::vector<RigidBody> &instances) {
    static bool doAdd = false; // flag to add RigidBody

    ImGui::Begin("Manager");
    if (ImGui::Button("Add circle")) {
        doAdd = true;
    }
    ImGui::End();


    if (doAdd) {
        RigidBody rb(1.0f, Vec3(0.0f, 0.0f, 0.0f));
        instances.push_back(rb);
        doAdd = false;
    }
}

void editor(std::vector<RigidBody> &instances) {
    static int selectedInstance = -1;

    // Draw a list of instances
    ImGui::Begin("Rigid Body Editor");

    const char* listbox_items[instances.size()];
    for (int i = 0; i < instances.size(); i++) {
        listbox_items[i] = instances[i].toString().c_str();
    }


    ImGui::ListBox("Instances", &selectedInstance, listbox_items, instances.size());

    ImGui::End();

    // If an instance is selected, show its properties
    if (selectedInstance >= 0 && selectedInstance < instances.size()) {
        RigidBody& instance = instances[selectedInstance];

        ImGui::Begin("Instance Properties");

        // Edit position
        ImGui::InputFloat3("Position", &instance.pos.x);

        ImGui::End();
    }
}

#endif //CG_ENGINE_GUI_H
