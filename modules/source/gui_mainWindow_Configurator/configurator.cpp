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
        //render all elements and mark what we need to delete
        for(const auto& [id, block] : pipelineBlocks){
            if(block->render()) toDelete.push_back(id);
        }

        //delete process of the node
        for(int i=0; i<toDelete.size(); i++){
            delete pipelineBlocks[toDelete[i]];
            pipelineBlocks.erase(toDelete[i]);
        }


        //render the links
        for(int i=0; i<links.size(); i++){
            const std::pair <int, int> p = links[i];
            ImNodes::Link(i, p.first, p.second);
        }

        ImNodes::EndNodeEditor();

        //update the links
        int tmp_start_attr, tmp_end_attr;
        if (ImNodes::IsLinkCreated(&tmp_start_attr, &tmp_end_attr)){
            bool linkOK = true;
            //map so its always output to input
            int end_attr   = (tmp_start_attr%100 < tmp_end_attr%100) ? (tmp_start_attr) : (tmp_end_attr);   
            int start_attr = (tmp_start_attr%100 < tmp_end_attr%100) ? (tmp_end_attr)   : (tmp_start_attr);   
            //get the nodes we are trying to connect
            pipelineBlock* nodeStart = pipelineBlocks[start_attr/100];
            pipelineBlock* nodeEnd   = pipelineBlocks[end_attr  /100];
            //we dont links to the same node
            if(nodeStart == nodeEnd) linkOK=false;
            //check if pin types are ok
            PinType startPin = nodeStart->getPinType(start_attr%100);
            PinType endPin = nodeEnd->getPinType(end_attr%100);
            if(startPin != endPin || startPin == PinType_none || endPin == PinType_none) linkOK=false;
            //if everything is ok create the link
            if(linkOK){
                //save it so we can draw it
                this->links.push_back(std::make_pair(start_attr, end_attr));
                std::cout << "New Link: " << start_attr << " to " << end_attr << std::endl;
                //link the elements so they know what buffer to push to
            }
        }

        ImGui::EndChild();
    }

private:
    std::vector <std::pair<int, int>> links;

    void renderAddPopup(){
        //Popup menu
        if(ImGui::BeginPopupContextWindow("nodeadd")){
            for(int i=0; i<pipelineBlocks_examples.size(); i++){
                if(pipelineBlocks_examples[i]->renderAdd()){
                    pipelineBlock* newBlock = pipelineBlock_createFuncs[i]();
                    pipelineBlocks[newBlock->getNodeID()] = newBlock;
                }
            }
            ImGui::EndPopup();
        }
    }
};

extern "C" mainWindow* moduleCreate_mainWindow() {
    return (mainWindow*) new mainWindow_configurator();
}