#include "mod-lib/gui_mainWindow.hpp"
#include "pipeline/pipeline.hpp"
#include "ImGui/imgui.h"

#include <iostream>


class mainWindow_configurator: public mainWindow{
public:
    std::string getModuleName() override { return "gui_mainWindow_configurator"; }

    std::string getWindowName() override { return "Config"; }

    void renderWindow() override{
        ImGuiIO& io = ImGui::GetIO();

        //The selector list
        int nodelistWidth = io.DisplaySize.x/3 < 300 ? (io.DisplaySize.x/3) : (300);

        ImGui::BeginChild("nodelist", ImVec2(300, 0), 0, ImGuiWindowFlags_MenuBar);
        for(int i=0;i<100; i++){
            ImGui::Text("test");
        }
        ImGui::EndChild();
        ImGui::SameLine();
        //The main menu
        ImGui::BeginChild("nodeEditor");
        for(int i=0;i<100; i++){
            ImGui::Text("Im 2nd the CHILD!");
        }
        ImGui::EndChild();
    }

private:

};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_configurator();
}