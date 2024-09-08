#include <gui/settings/numberSelector.hpp>
#include <iostream>
#include <raylib.h>
#include <math.h>

NumberSelector::NumberSelector(long long max, long long min, long long* val, const char* title):Setting(title){
    if(min >= max){
        std::cout << "ERROR: MIN IS BIGGER OR EQ TO MAX IN NumberSelector CONSTRUCTOR!" << std::endl;
        std::exit(-1);
    }
    this->max = max;
    this->min = min;
    this->val = val;
    this->posX = posX;
    this->posY = posY;
    //calc number of decimals
    int maxval = abs(max) < abs(min) ? abs(min) : abs(max);

    if(min < 0){
        this->hasSign = true;
        numberOfDecimals++;
    }

    while(maxval / 10 != 0){
        numberOfDecimals++;
        maxval = maxval / 10;
    }
}

void NumberSelector::render(int posX, int posY, int my, int mx) {
    long long num = * this->val;
    for(int i=numberOfDecimals-1; i>=0; i--){
        int elStartX = posX + i * (SETTING_NUMBERSELECTOR_ELEMENT_WIDTH + SETTING_NUMBERSELECTOR_ELEMENT_SPACE);
        bool hoverX = (mx > elStartX) && (mx < elStartX + SETTING_NUMBERSELECTOR_ELEMENT_WIDTH);
        //render background
        if(hoverX && (my > posY) && (my < posY+SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT)){
            //highlight top or bottom
            bool top = (my < posY+(SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT/2));
            DrawRectangle(elStartX, posY                                          , SETTING_NUMBERSELECTOR_ELEMENT_WIDTH, (SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT/2), top ? COLOR_HOVER : COLOR_IDLE);
            DrawRectangle(elStartX, posY+(SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT/2), SETTING_NUMBERSELECTOR_ELEMENT_WIDTH, (SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT/2), top ? COLOR_IDLE  : COLOR_HOVER);
        }else{
            DrawRectangle(elStartX, posY, SETTING_NUMBERSELECTOR_ELEMENT_WIDTH, SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT, COLOR_IDLE); //USE DEFAULT COLORS
        }
        //render Number
        if(hasSign){
            if(i == 0){
                DrawText(TextFormat(*this->val < 0 ? "-" : "+"), elStartX+4, posY+2, SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT-4, RAYWHITE);
                continue;
            }
        }
        DrawText(TextFormat("%d", abs((int)num)%10), elStartX+4, posY+2, SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT-4, RAYWHITE);
        num /= 10;
    }
}

void NumberSelector::action(int posX, int posY, int my, int mx, int button){
    if(my < posY || my > posY+SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT || mx < posX) return;
    long long mod = 1;
    for(int i=numberOfDecimals-1; i>=0; i--){
        int elStartX = posX + i * (SETTING_NUMBERSELECTOR_ELEMENT_WIDTH + SETTING_NUMBERSELECTOR_ELEMENT_SPACE);
        //check if currrent element is selected
        bool hoverX = (mx > elStartX) && (mx < elStartX + SETTING_NUMBERSELECTOR_ELEMENT_WIDTH);
        if(!hoverX){
            //go over to next element
            mod *= 10;
            continue;
        };
        long long buf = * this->val; // create copy if we go out of bounds
        bool top = (my < posY+(SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT/2));
        //to we need to count or round ?
        if(button == 0){
            //sub / add modifier to it
            *this->val += (top ? mod : -mod);
        }else if(button == 1){
            //round to next higher decimal place
            *this->val /= mod*10;
            *this->val *= mod*10;
        }
        //undo if we are out of bounds
        if(*this->val < min || *this->val > max){
            *this->val = buf;
        }
        break; //there can only be once element at a time so exit for performance
    }
}

int NumberSelector::getHeight(){
    return(SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT);
}

int NumberSelector::getWidth(){
    return((SETTING_NUMBERSELECTOR_ELEMENT_WIDTH+SETTING_NUMBERSELECTOR_ELEMENT_SPACE) * this->numberOfDecimals);
}