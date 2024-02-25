#include <gui/setting.hpp>
#include <gui/def.hpp>

#define SETTING_NUMBERSELECTOR_ELEMENT_WIDTH 26
#define SETTING_NUMBERSELECTOR_ELEMENT_SPACE 2
#define SETTING_NUMBERSELECTOR_ELEMENT_HEIGHT SETTING_HEIGHT

class NumberSelector : public Setting{
private:
    long long max, min;
    long long* val;
    int numberOfDecimals = 1;
    int posX, posY;
    bool hasSign = false;

public:
    NumberSelector(long long max, long long min, long long* val);

    //interface
    void render(int posX, int posY, int my, int mx) override;
    void action(int posX, int posY, int my, int mx, int button) override;
    int getHeight() override;
    int getWidth() override;
};