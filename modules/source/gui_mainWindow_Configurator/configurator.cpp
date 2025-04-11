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

        //render all the nodes
        ImGui::BeginChild("nodelist");
        ImNodes::BeginNodeEditor();

        renderAddPopup();

        std::vector <int> toDelete;
        //render all elements
        for(int i=0; i<pipelineBlocks.size(); i++){
            if(pipelineBlocks[i]->render()) toDelete.push_back(i);
        }
        //remove all of them that a scheduled to deletion
        for(int i=toDelete.size()-1; i>=0; i--){
            pipelineBlocks.erase(pipelineBlocks.begin() + toDelete[i]);         //TODO unique ptr doesnt call destructor..
        }

        ImNodes::EndNodeEditor();
        ImGui::EndChild();
    }

private:
    void renderAddPopup(){
        //Popup menu
        if(ImGui::BeginPopupContextWindow("nodeadd")){
            for(int i=0; i<pipelineBlocks_examples.size(); i++){
                if(pipelineBlocks_examples[i]->renderAdd()){
                    pipelineBlocks.emplace_back(pipelineBlock_createFuncs[i]());
                }
            }
            ImGui::EndPopup();
        }
    }
};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_configurator();
}