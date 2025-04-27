#pragma once
#include "./modules.hpp"

#include <imnodes/imnodes.h>
#include <ImGui/imgui.h>
#include <string>

#include <iostream>
#include <map>
#include <vector>
#include <tuple>


enum PinType{
    PinType_none,
    PinType_value,
    PinType_samples,
    PinType_spectrum
};

unsigned int nodeTypeColors[] = {IM_COL32(144, 144, 144, 255), IM_COL32(245, 144, 39, 255), IM_COL32(200,162,200,255), IM_COL32(53, 155, 12, 255)};

void setNodeStyle(PinType type){
    ImNodes::PushColorStyle(ImNodesCol_Pin, nodeTypeColors[type]);
}   


class pipelineBlock : public Module{
public:
    pipelineBlock(){
        this->nodeID = nodeIDcnt;
        nodeIDcnt++;
    }
    virtual ~pipelineBlock() = default;

    virtual bool render()    {return false;}     //returns true if should be deleated
    virtual bool renderAdd() {return false;}     //returns true if it was selected

    virtual PinType getPinType(int id) {return PinType_none;}

    virtual void run() { std::cout << "WARN (" << headline << "): forgot to override run function!" << std::endl;}

    void setBufferSource(int index, void* buf) {
        if(buf == nullptr){
            std::cout << "UNEXPECTED ERROR: got nullprt as Target Buffer!" << std::endl;
            return;
        }
        this->sourceBuffers[index] = buf;
    }
    void removeBufferSource(int index){
        this->sourceBuffers.erase(index);
    }
    virtual void* getBuffer(int index) {return nullptr;}

    int getNodeID() {return this->nodeID;}

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


    std::map<int, void*> sourceBuffers;
};