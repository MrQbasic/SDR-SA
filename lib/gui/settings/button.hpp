#pragma once

#include <gui/setting.hpp>
#include <gui/def.hpp>
#include <raylib.h>
#include <iostream>

#include <functional>

class Button : public Setting {
private:
    std::function<void()> cb;
    const char* text;
    int width;
    int heigth = SETTING_HEIGHT;
    bool latching;
    bool* state;

    Color* textcols;
    int* colstate;

public:
    
    Button(const char* text, bool* state, const char* title):Setting(title){
        this->latching = true;
        this->width = MeasureText(text, SETTING_HEIGHT-2*SETTING_PADDING) + 2*SETTING_PADDING;
        this->state = state;
        this->text = text;
        this->textcols = nullptr;
    }

    Button(const char* text, std::function<void()> callBack, const char* title):Setting(title){
        this->latching = false;
        this->width = MeasureText(text, SETTING_HEIGHT-2*SETTING_PADDING) + 2*SETTING_PADDING;
        this->cb = callBack;
        this->text = text;
        this->textcols = nullptr;
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
            if(latching){
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
            if(latching){
                *state ^= 1;
            }else{
                cb();
            }
        }
    }

    int getHeight() {return this->heigth;}
    int getWidth()  {return this->width;}

};