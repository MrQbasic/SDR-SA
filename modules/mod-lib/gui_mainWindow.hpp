#pragma once
#include "./modules.hpp"


class mainWindow : public Module{
public:
    virtual std::string getWindowName(){
        return "window!";
    }
    virtual void renderWindow(){

    }
    virtual std::string getModuleName(){
        return "fuck!";
    }
};