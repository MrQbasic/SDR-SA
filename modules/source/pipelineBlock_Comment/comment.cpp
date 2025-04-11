#include "mod-lib/pipelineBlock.hpp"

#include <memory>

class pipelineBlock_comment : public pipelineBlock{
public:
    ~pipelineBlock_comment(){
        std::cout << "Destructor called!" << std::endl;
    }

    pipelineBlock_comment(){
        this->headline = "Comment";
    }

    std::string getModuleName() override {return "pipelineBlock_comment";}

    bool render() override {
        //do generic setup
        generic_renderStart();

        ImGui::InputTextMultiline("", text, 1024, ImVec2(300, 400));

        return generic_renderEnd();
    }

    bool renderAdd() override{
        return ImGui::MenuItem("Comment");
    }


private:
    char* text = new char[1024] {"This is a comment...."};
};



extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_comment();
}