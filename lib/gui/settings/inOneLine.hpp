#include <gui/setting.hpp>
#include <gui/def.hpp>

#include <vector>

class inOneLine : public Setting{
private:
    std::vector<Setting*> settings;

public:
    inOneLine(std::vector<Setting*> settings, const char* title):Setting(title){
        this->settings = settings;
    }

    void render(int posX, int posY, int my, int mx) override{
        settings[0]->render(posX, posY, my, mx);
        int settingX = posX + settings[0]->getWidth() + SETTING_PADDING;
        for(int i=1; i<settings.size(); i++){
            settings[i]->render(settingX, posY, my, mx);
            settingX += settings[i]->getWidth() + SETTING_PADDING;
        }
    }

    void action(int posX, int posY, int my, int mx, int button) override{
        settings[0]->action(posX, posY, my, mx, button);
        int settingX = posX + settings[0]->getWidth() + SETTING_PADDING;
        for(int i=1; i<settings.size(); i++){
            settings[i]->action(settingX, posY, my, mx, button);
            settingX += settings[i]->getWidth() + SETTING_PADDING;
        }
    }

    int getHeight() override {return SETTING_HEIGHT;}
    int getWidth() override {
        int width = 0;
        for(int i=0; i<settings.size(); i++){
            int entryWidth = settings[i]->getWidth();
            if(entryWidth == -100) continue;
            width += settings[i]->getWidth() + SETTING_PADDING;
        }
        width -= SETTING_PADDING;
        return width;
    }
    
};