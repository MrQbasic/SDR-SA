#include "mod-lib/gui_mainWindow.hpp"
#include "pipeline/pipeline.hpp"
#include "ImGui/imgui.h"

#include <imnodes/imnodes.h>

#include <iostream>


class mainWindow_configurator: public mainWindow{
public:
    std::string getModuleName() override { return "gui_mainWindow_configurator"; }

    std::string getWindowName() override { return "Config"; }

    void renderWindow() override{
        ImGuiIO& io = ImGui::GetIO();

        //The selector list

        ImGui::BeginChild("nodelist");
        
        
        ImNodes::BeginNodeEditor();


        piplineBlocks[0]->render();

        /*
        ImNodes::BeginNode(1);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Test Node");
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(2);
        ImGui::Text("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(3);
        ImGui::Indent(40);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        */

        ImNodes::EndNodeEditor();

        ImGui::EndChild();
    }

private:

};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_configurator();
}