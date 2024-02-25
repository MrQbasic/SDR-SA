#include <gui/setting.hpp>
#include <gui/menu.hpp>
#include <gui/def.hpp>

class Submenu : public Setting{
private:
    bool* displayState;
    i_MenuFrame* frame;

public:
    Submenu(i_MenuFrame* frame, bool* state){
        this->displayState = state;
        this->frame = frame;
    }
    
    void render(int posX, int posY, int my, int mx) override{
        if(!*this->displayState) return;

        this->frame->setPosY(posY-MENU_HEIGHT-MENU_FRAME_Y_SPACE*3);
        this->frame->render(my, mx);
    }
    
    void action(int posX, int posY, int my, int mx, int button) override{
        this->frame->action(my, mx, button);
    }


    int getHeight() override {return -MENU_FRAME_Y_SPACE;}
    int getWidth() override  {return 0;}


};