#include <gui/menu.hpp>
#include <gui/def.hpp>
#include <gui/setting.hpp>

#include <gui/settings/text.hpp>
#include <gui/settings/button.hpp>
#include <gui/settings/inOneLine.hpp>
#include <gui/settings/dummy.hpp>

#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>

#include <raylib.h>

int MenuEntry::idCnt = 0;
int MenuEntry::selectedId = -1;
int MenuEntry::xPosCnt = 0;

MenuEntry::MenuEntry(const char* Title, MenuFrame* frame){
    this->Title = Title;
    this->frame = frame;
    this->xPos  = xPosCnt;
    
    this->frame->setPosX(this->xPos);
    this->frame->setPosY(MENU_Y_START+MENU_HEIGHT);

    xPosCnt += MeasureText(Title, MENU_TEXT_SIZE);
    this->width = xPosCnt - xPos;
    xPosCnt += MENU_SPACE + 2 * MENU_TEXT_X_OFFSET;
    this->ID = ++idCnt;
}

void MenuEntry::renderBG(){
    int sw = GetScreenWidth();
    DrawRectangle(MENU_X_START, MENU_Y_START, sw, MENU_HEIGHT, MENU_BG_COLOR);
}

void MenuEntry::render(int mx, int my){
    bool hover    = (mx > MENU_X_START+this->xPos) && (mx < MENU_X_START+this->width+this->xPos) && (my > MENU_Y_START) && (my < MENU_Y_START + MENU_HEIGHT);
    bool selected = (selectedId == this->ID);
    DrawRectangle(MENU_X_START + xPos, MENU_Y_START, this->width + 2 * MENU_TEXT_X_OFFSET, MENU_HEIGHT, hover ? MENU_COLOR_HOVER : (selected ? MENU_COLOR_SELECTED : MENU_COLOR));
    DrawText(this->Title, MENU_X_START + xPos + MENU_TEXT_X_OFFSET, MENU_Y_START + MENU_TEXT_Y_OFFSET, MENU_TEXT_SIZE, MENU_TEXT_COLOR);
    if(!selected) return;
    //if current menu is selected render Frame
    this->frame->render(my, mx);

}

void MenuEntry::action(int mx, int my, int button){
    bool overEntry = (mx > MENU_X_START+this->xPos) && (mx < MENU_X_START+this->width+this->xPos) && (my > MENU_Y_START) && (my < MENU_Y_START + MENU_HEIGHT);
    if(overEntry){
        if(selectedId == this->ID){
            selectedId = -1;
        }else{
            selectedId = this->ID;
        }
    }
    //check if we need to pass the action check down to the frame //TODO: optim check if mouse if over frame firstly ?
    if(selectedId == this->ID){
        this->frame->action(my, mx, button);
    }
}

MenuFrame::MenuFrame(std::vector<Setting*> settings){
    this->settings = settings;
    this->width = 0; 
    this->height = 0;
    //auto find width and height
    for(int i=0; i<settings.size(); i++){
        int spacer = 100;
        if(settings[i]->getTitle()[0] == '\n'){
            spacer = 0;
        }
        int widthOfEntry = settings[i]->getWidth() + MeasureText(settings[i]->getTitle(), SETTING_HEIGHT-4) + spacer; // text + setting + padding inbetween
        if(this->width < widthOfEntry) this->width = widthOfEntry;
        //
        this->height += settings[i]->getHeight() + MENU_FRAME_Y_SPACE;
    }
    this->width  += MENU_FRAME_PADDING*2;
    this->height += -MENU_FRAME_Y_SPACE + MENU_FRAME_PADDING*2;
}

void MenuFrame::render(int my, int mx){
    DrawRectangle(posX, posY, this->width, this->height, MENU_BG_COLOR);
    Rectangle outline = {(float) posX,(float) posY,(float) this->width,(float) this->height};
    DrawRectangleLinesEx(outline, 3, COLOR_SELECTED);

    int currentY = posY + MENU_FRAME_PADDING;
    for(int i=0; i<this->settings.size(); i++){
        DrawText(this->settings[i]->getTitle(), posX+MENU_FRAME_PADDING, currentY+2, SETTING_HEIGHT-4, RAYWHITE);
        this->settings[i]->render(posX + width - MENU_FRAME_PADDING - this->settings[i]->getWidth(), currentY, my, mx);
        currentY += this->settings[i]->getHeight() + MENU_FRAME_Y_SPACE;
    }
}

void MenuFrame::action(int my, int mx, int button){
    int currentY = posY + MENU_FRAME_PADDING;
    for(int i=0; i<this->settings.size(); i++){
        this->settings[i]->action(posX + width - MENU_FRAME_PADDING - this->settings[i]->getWidth(), currentY, my, mx, button);
        currentY += this->settings[i]->getHeight() + MENU_FRAME_Y_SPACE;
    }
}

int MenuFrame::getHeight() {return(this->height);}
int MenuFrame::getWidth() {return(this->width);}

int MenuFrame::getPosX() {return this->posX;}
int MenuFrame::getPosY() {return this->posY;}

void MenuFrame::setPosX(int x) {this->posX = x;}
void MenuFrame::setPosY(int y) {this->posY = y;}



MenuFrame::MenuFrame(RTLSDR* sdr){
    //setup settings
    //-disconnect button
    auto discon = [sdr](){std::cout << "WHY TF IS THE DISCON CALLED ?" << std::endl;sdr->state=2;};
    this->settings.push_back(new Button("Disconnect", discon, "\n"));
    //-gain settings
    char* gainText = new char[100];
    double gain = (double) sdr->changeGain(-10000) / 10.0;
    std::sprintf(gainText, "%.1f", gain);

    auto incGain = [sdr, gainText](){
        double newGain = (double) sdr->changeGain(1) / 10.0;
        std::sprintf(gainText, "%.1f", newGain);
        std::cout << "inc changed @: " << sdr << std::endl;
    };
    auto decGain = [gain, sdr, gainText](){
        double newGain = (double) sdr->changeGain(-1) / 10.0;
        std::sprintf(gainText, "%.1f", newGain);
        std::cout << "dec changed @: " << sdr << std::endl;

    };
    std::vector<Setting*> gainSetting;
    gainSetting.push_back(new Button("+",incGain, "\n"));
    gainSetting.push_back(new Button("-",decGain, "\n"));
    gainSetting.push_back(new Text(gainText,"\n"));
    this->settings.push_back(new inOneLine(gainSetting, "Gain:"));
    //auto find width and height
    this->width = 0; 
    this->height = 0;
    for(int i=0; i<settings.size(); i++){
        int spacer = 100;
        if(settings[i]->getTitle()[0] == '\n'){
            spacer = 0;
        }
        int widthOfEntry = settings[i]->getWidth() + MeasureText(settings[i]->getTitle(), SETTING_HEIGHT-4) + spacer; // text + setting + padding inbetween
        if(this->width < widthOfEntry) this->width = widthOfEntry;
        //
        this->height += settings[i]->getHeight() + MENU_FRAME_Y_SPACE;
    }
    this->width  += MENU_FRAME_PADDING*2;
    this->height += -MENU_FRAME_Y_SPACE + MENU_FRAME_PADDING*2;
}