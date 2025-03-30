#include "mod-lib/pipelineBlock.hpp"

#include <imnodes/imnodes.h>
#include <string>

class pipelineBlock_comment : public pipelineBlock{
    std::string getModuleName() {return "piplineBlock_comment";}

    void render() override {
        ImNodes::BeginNode(this->nodeID);
        
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Comment");
        ImNodes::EndNodeTitleBar();

        ImGui::InputTextMultiline("", text, 1024);

        ImNodes::EndNode();
    }

private:
    char* text = new char[1024];
};



extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_comment();
}