#include "mod-lib/gui_mainWindow.hpp"
#include "ImGui/imgui.h"
#include "implot/implot.h"

class mainWindow_graph: public mainWindow{
public:
    std::string getModuleName() override { return "gui_mainWindow_settings";}
    std::string getWindowName() override { return "Graph";}

    void renderWindow() override{
        int maxWidth = ImGui::GetContentRegionAvail().x;
        ImPlot::BeginPlot("###plot", ImVec2(maxWidth * 0.7, -1));
        ImPlot::EndPlot();
        

    }

};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_graph();
}