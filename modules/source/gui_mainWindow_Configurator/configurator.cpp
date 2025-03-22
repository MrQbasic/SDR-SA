#include "mod-lib/gui_mainWindow.hpp"

#include "ImGui/imgui.h"

#include <iostream>


class mainWindow_configurator: public mainWindow{
public:
    std::string getModuleName() override { return "gui_mainWindow_configurator"; }

    std::string getWindowName() override { return "Config"; }

    void renderWindow() override{
        ImGui::Text("Test");
    }

private:
    bool displayDemo = false;

};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_configurator();
}