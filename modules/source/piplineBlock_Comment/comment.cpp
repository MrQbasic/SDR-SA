#include "mod-lib/pipelineBlock.hpp"


class pipelineBlock_comment : public pipelineBlock{
    std::string getModuleName() {return "piplineBlock_comment";}


};



extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_comment();
}