#include <gui/setting.hpp>
#include <gui/def.hpp>
#include <raylib.h>

class Text : public Setting {
private:
    char* text;

public:
    Text(char* text, const char* title):Setting(title){
        this->text = text;
    }

    void render(int posX, int posY, int my, int mx) override{
        const char* text = TextFormat("%s",this->text);
        int textWidth = MeasureText(text, SETTING_HEIGHT-4);
        DrawRectangle(posX, posY, textWidth + 4 , SETTING_HEIGHT, COLOR_IDLE);
        DrawText(text, posX+2, posY+2, SETTING_HEIGHT-4, RAYWHITE);
    }
    void action(int posX, int posY, int my, int mx, int button) override {}

    int getHeight() override {
        return SETTING_HEIGHT;
    }
    int getWidth() override{
        const char* text = TextFormat("%s",this->text);
        int textWidth = MeasureText(text, SETTING_HEIGHT-4);
        return textWidth+4;
    }

};