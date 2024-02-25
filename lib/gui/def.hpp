#include <raylib.h>


#define COLOR_IDLE          GRAY
#define COLOR_SELECTED      LIGHTGRAY
#define COLOR_HOVER         DARKPURPLE
#define COLOR_BACKGROUND    DARKGRAY

#define MENU_X_START    0
#define MENU_Y_START    0
#define MENU_HEIGHT     30
#define MENU_COLOR              COLOR_IDLE
#define MENU_COLOR_SELECTED     COLOR_SELECTED
#define MENU_COLOR_HOVER        COLOR_HOVER
#define MENU_TEXT_COLOR         RAYWHITE
#define MENU_TEXT_SIZE          26
#define MENU_TEXT_X_OFFSET      4
#define MENU_TEXT_Y_OFFSET (MENU_HEIGHT - MENU_TEXT_SIZE) / 2
#define MENU_SPACE              5
#define MENU_BG_COLOR           COLOR_BACKGROUND

#define MENU_FRAME_PADDING      10
#define MENU_FRAME_Y_SPACE      5

#define SETTING_HEIGHT          40
#define SETTING_PADDING         2

#define GRAPH_X_DIVS            15
#define GRAPH_Y_DIVS            10
#define GRAPH_PADDING           40
#define GRAPH_TEXT_SIZE         10
#define GRAPH_TEXT_COLOR        RAYWHITE
#define GRAPH_FREQ_DECMIALS     1