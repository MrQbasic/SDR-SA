#pragma once

#include <gui/setting.hpp>
#include <gui/def.hpp>
#include <raylib.h>
#include <iostream>

#include <functional>

class Button : public Setting {
private:
    std::function<void()> cb = nullptr;
    const char* text;
    int width;
    int heigth = SETTING_HEIGHT;
    bool* state;

    Color* textcols = nullptr;
    int* colstate;

public:
    Button(const char* text, std::function<void()> callBack, bool* state, const char* title):Setting(title){
        this->width = MeasureText(text, SETTING_HEIGHT-2*SETTING_PADDING) + 2*SETTING_PADDING;
        this->cb = callBack;
        this->state = state;
        this->text = text;
    }


    void setupColorText(Color* cols, int* colstate){
        this->textcols = cols;
        this->colstate = colstate;
    }

    void render(int posX, int posY, int my, int mx) override{
        //check if we hover over the button
        if(mx > posX && mx < posX+width && my > posY && my < posY+heigth){
            DrawRectangle(posX, posY, width, heigth, COLOR_HOVER);
        }else{
            if(state != nullptr){
                if(*state){
                    DrawRectangle(posX, posY, width, heigth, COLOR_SELECTED);
                }else{
                    DrawRectangle(posX, posY, width, heigth, COLOR_IDLE);
                }
            }else{
                DrawRectangle(posX, posY, width, heigth, COLOR_IDLE);
            }
        }
        DrawText(this->text, posX+SETTING_PADDING, posY+SETTING_PADDING, SETTING_HEIGHT-2*SETTING_PADDING, textcols == nullptr ? RAYWHITE : textcols[*colstate]);

    }

    void action(int posX, int posY, int my, int mx, int button) override{
        if(mx > posX && mx < posX+width && my > posY && my < posY+heigth){
            if(state != nullptr){
                *state ^= 1;
            }
            if(cb != nullptr){
                cb();
            }
        }
    }

    int getHeight() {return this->heigth;}
    int getWidth()  {return this->width;}

};