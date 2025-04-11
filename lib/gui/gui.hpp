#include "ImGui/imgui.h"

#include "mod-lib/gui_mainWindow.hpp"

#include <memory>
#include <vector>
#include <tuple>
#include <iostream>
#include <dlfcn.h>
#include <filesystem>

std::vector<std::unique_ptr<mainWindow>> mainWindows;

ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

ImGuiTabBarFlags mainWindowSelectorFlags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;

void setupGui(){
    std::string path = "./build/modules/source";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::string path = entry.path();
            //check if its even the file type we need!
            if(path.find(".so") == std::string::npos) continue;
            if(path.find("gui_mainWindow_") == std::string::npos) continue;
            //try loading the module   
            void* handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            if(!handle){
                std::cerr << "Faild to load Module: " << path << std::endl;
                std::cerr <<  dlerror() << std::endl;
                continue;
            }
            dlerror();
            //getting the factory function
            typedef mainWindow* (*CreateModuleFunc)();
            CreateModuleFunc createFunc = (CreateModuleFunc) dlsym(handle, "moduleCreate_mainWindow");
            if (!createFunc) {
                std::cerr << "Failed to get createModule function: " << dlerror() << "\n";
                dlclose(handle);
                continue;
            }
            //save the window
            mainWindows.emplace_back(createFunc());
            std::cout << "Module added: " << path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error opening directory: " << e.what() << std::endl;
    }
}


void renderGui(){
    //make the window fullscreen
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    //render the window
    ImGui::Begin("mainWindow", nullptr, mainWindowFlags);
    ImGui::BeginTabBar("mainWindowSelector", mainWindowSelectorFlags);
        //do it for all loaded mainWindows
        for(auto& window : mainWindows){
            if(ImGui::BeginTabItem(window->getWindowName().c_str())){
                //render the window
                window->renderWindow();
                ImGui::EndTabItem();
            }
        }
    ImGui::EndTabBar();
    ImGui::End();
}
