#pragma once
#include <gui/def.hpp>

class Setting{
public:
    //called for each frame (mouse for hover highlight)
    virtual void render(int posX, int posY, int my, int mx) = 0 ;
    //only called when something is actually pressed
    virtual void action(int posX, int posY, int my, int mx, int button) = 0;

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};