#pragma once
#include "./modules.hpp"

#include <iostream>

class pipelineBlock : public Module{
public:
    pipelineBlock(){
        this->nodeID = nodeIDcnt;
        nodeIDcnt++;
        std::cout << "super called!" << std::endl;
    }

    virtual void render() {return;}


private:
    inline static int nodeIDcnt = 1;

protected:
    int nodeID;

};