#pragma once
#include <ImGui/imgui.h>


bool inline setting_easyMode = true;

bool inline setting_renderDemoWindow = false;

inline void renderSettingsMenu(){
    ImGui::Checkbox("Easy mode", &setting_easyMode);
    ImGui::Checkbox("Demo Wimdow", &setting_renderDemoWindow);

    if(setting_renderDemoWindow){
        ImGui::ShowDemoWindow();
    }
}