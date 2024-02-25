#include <gui/setting.hpp>
#include <gui/def.hpp>

#pragma once

class DummySetting : public Setting{

    void render(int posX, int posY, int my, int mx) override {}
    void action(int posX, int posY, int my, int mx, int button) override {}

    int getHeight() override {return SETTING_HEIGHT;}
    int getWidth() override  {return -100;} //WORKAROUND TO GET RID OF SPACEING IBETWEEN
};