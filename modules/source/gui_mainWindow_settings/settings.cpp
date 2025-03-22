#include "mod-lib/gui_mainWindow.hpp"

#include "ImGui/imgui.h"


class mainWindow_settings: public mainWindow{
public:
    std::string getModuleName() override { return "gui_mainWindow_settings"; }

    std::string getWindowName() override { return "Settings"; }

    void renderWindow() override{
        ImGui::Checkbox("Demo Window", &displayDemo);
        if(displayDemo) ImGui::ShowDemoWindow();
    }

private:
    bool displayDemo = false;

};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_settings();
}