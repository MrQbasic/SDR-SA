#pragma once
#include "./modules.hpp"

#include <imnodes/imnodes.h>
#include <ImGui/imgui.h>
#include <string>

#include <iostream>

class pipelineBlock : public Module{
public:
    pipelineBlock(){
        this->nodeID = nodeIDcnt;
        nodeIDcnt++;
        std::cout << "super called!" << std::endl;
    }

    

    virtual bool render()    {return false;}     //returns true if should be deleated
    virtual bool renderAdd() {return false;}     //returns true if it was selected

protected:
    void generic_renderStart() {
        ImNodes::BeginNode(this->nodeID);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(this->headline.c_str());
        ImNodes::EndNodeTitleBar();

    }
    
    //returns true on close
    bool generic_renderEnd() {  
        ImNodes::EndNode();
        return (ImNodes::IsNodeSelected(this->nodeID) && ImGui::IsKeyPressed(ImGuiKey_Delete));
    }

private:


    inline static int nodeIDcnt = 1;

protected:
    int nodeID;
    std::string headline = "pipelineBlock";
};