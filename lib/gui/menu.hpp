#pragma once

#include <string>
#include <gui/setting.hpp>
#include <sdrs/rtl.hpp>


class i_MenuFrame{
public:
    virtual void render(int my, int mx) {};
    virtual void action(int my, int mx, int button) {};
    virtual void setPosX(int x) = 0;
    virtual void setPosY(int y) = 0; 
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual int getPosX() = 0;
    virtual int getPosY() = 0;
};

class MenuFrame : public i_MenuFrame{
private:
    int width, height, posX, posY;

    Setting**     settings;
    const char** titles;
    int           numberOfSettings;

public:

    MenuFrame(Setting* settings[], const char* titles[], int numberOfSettings);
    MenuFrame(RTLSDR* sdr);
    
    void render(int my, int mx);
    void action(int my, int mx, int button);
    void setPosX(int x);
    void setPosY(int y); 
    int getWidth();
    int getHeight();
    int getPosX();
    int getPosY();

};


class ConditionalMenuFrame : public i_MenuFrame{
private:
    MenuFrame** frames;
    int cnt;
    int* state;
    int posX = 0;
    int posY = 0;

public:    
    ConditionalMenuFrame(MenuFrame** frames, int cnt, int* state){
        this->frames = frames;
        this->cnt = cnt;
        this->state = state;
    }
    void render(int my, int mx) override{
        frames[*state]->setPosX(posX);
        frames[*state]->setPosY(posY);
        frames[*state]->render(my, mx);
    }
    void action(int my, int mx, int button) override{
        frames[*state]->action(my, mx, button);
    }
    void setPosX(int x) override{ this->posX = x;}
    void setPosY(int y) override{ this->posY = y;}
    int getPosX() override{ return this->posX; }
    int getPosY() override{ return this->posY; }

    int getWidth()  override{ return frames[*state]->getWidth();}
    int getHeight() override{ return frames[*state]->getHeight();}
};


class MenuEntry{
private:
    //id for menu selection
    static int selectedId;
    static int idCnt;
    int ID;
    //rendering things
    static int xPosCnt;
    int xPos;
    int width;
    //
    const char* Title;
    MenuFrame* frame;


public:
    MenuEntry(const char* Title, MenuFrame* frame);

    void render(int mx, int my);

    void action(int mx, int my, int button);

    static void renderBG();
};





