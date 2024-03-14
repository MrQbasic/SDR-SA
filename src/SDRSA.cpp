#include <raylib.h>

#include <gui/menu.hpp>
#include <gui/setting.hpp>
#include <gui/graph.hpp>
#include <gui/def.hpp>

#include <gui/settings/button.hpp>
#include <gui/settings/dummy.hpp>
#include <gui/settings/numberSelector.hpp>
#include <gui/settings/submenu.hpp>

#include <sdr.hpp>
#include <sdrs/rtl.hpp>

#include <iostream>
#include <vector>
#include <string>
 
long long freqStart =  90 * 1000 * 1000; //80MHZ
long long freqEnd   = 100 * 1000 * 1000; //115MHZ
long long dbTop = -70;
long long dbBottom = -150;

bool paused = false;

int main(){
    //init GX
    InitWindow(1280, 720, "SDR - SA");
    SetTargetFPS(60);

    //Gerneal Dimension Settings) spacer = 0
    std::vector<Setting*> settingsDimension;
    settingsDimension.push_back(new NumberSelector(99999999999, 0,    &freqStart, "START:"));
    settingsDimension.push_back(new NumberSelector(99999999999, 0,    &freqEnd,   "END:"));
    settingsDimension.push_back(new NumberSelector(99,          -150, &dbTop,     "DB TOP:"));
    settingsDimension.push_back(new NumberSelector(99,          -150, &dbBottom,  "DB BOTTOM:"));
    MenuFrame f_Dim = MenuFrame(settingsDimension);
    MenuEntry m_Dim = MenuEntry("SPECTRUM", &f_Dim);


    //scan for SDRs
    int sdrCnt = 0;
    const char** sdrNames;
    RTLSDR::getSDRs(&sdrCnt, &sdrNames);

    //main menu
    bool* sdrSelect       = new bool[sdrCnt];
    bool* sdrSelectBuffer = new bool[sdrCnt];
    std::vector<Setting*> settingsSdr;

    Color sdrButtonColors[] = {RAYWHITE, BLUE};
    Color graphColors[] = {BLUE, RED, GREEN};

    //submenu
    i_MenuFrame**   f_Sdr_submenu = new i_MenuFrame*[sdrCnt];

    SDR** sdrs = new SDR*[sdrCnt];
    Graph** graphs = new Graph*[sdrCnt];

    for(int i=0; i<sdrCnt; i++){
        sdrs[i] = new RTLSDR(i);
        graphs[i] = new Graph(graphColors[i], sdrs[i]);
        MenuFrame** subsubframes = new MenuFrame*[2];
        //construct SDR selector
        sdrSelect[i] = false;
        sdrSelectBuffer[i] = false;
        Button* sdrButton = new Button(sdrNames[i], &(sdrSelect[i]), "\n");
        sdrButton->setupColorText(sdrButtonColors, &(sdrs[i]->state));
        settingsSdr.push_back(sdrButton);
        //construct sdr connect menu
        auto sdrPrt = sdrs[i];
        auto conFunction = [sdrPrt, subsubframes](){
            sdrPrt->init();
            subsubframes[1] = new MenuFrame((RTLSDR*)sdrPrt);   // the other menu is created when the init is called
        };
        std::vector<Setting*> connectSettings = {new Button("Connect",conFunction, "\n")};
        //SubSubmenu
        subsubframes[0] = new MenuFrame(connectSettings);
        f_Sdr_submenu[i] = new ConditionalMenuFrame(subsubframes,2,&(sdrs[i]->state));
        settingsSdr.push_back(new Submenu(f_Sdr_submenu[i], &sdrSelect[i], "\n"));
    }
    

    //SDR selector Menu
    settingsSdr.insert(settingsSdr.begin(), new Button("PAUSE ALL", &paused, "\n"));
    MenuFrame* f_Sdr = new MenuFrame(settingsSdr);
    MenuEntry m_Sdr = MenuEntry("SDR", f_Sdr);

    
    //calculate position of submenus
    int sdrMenuPosX = f_Sdr->getPosX();
    for(int i=0; i<sdrCnt; i++){
        if( sdrMenuPosX > GetScreenWidth()/2){
            f_Sdr_submenu[i]->setPosX(sdrMenuPosX - f_Sdr_submenu[i]->getWidth());
        }else{
            f_Sdr_submenu[i]->setPosX(sdrMenuPosX + f_Sdr->getWidth());
        }
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        //
        int mx = GetMouseX();
        int my = GetMouseY();
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        
        //check for an action
        if(IsMouseButtonPressed(0)){
            m_Dim.action(mx, my, 0);
            m_Sdr.action(mx, my, 0);
        }
        if(IsMouseButtonPressed(1)){
            m_Dim.action(mx, my, 1);
            m_Sdr.action(mx, my, 1);
        }
        if(IsKeyPressed(KEY_SPACE)){
            paused ^= 1;
        }
        
        //sdrmenu submenu
        int sdrSubmenuChanges = 0;
        for(int i=0; i<sdrCnt; i++){
            if(sdrSelect[i])sdrSubmenuChanges++;
        }
        if(sdrSubmenuChanges > 1){
            for(int i=0; i<sdrCnt; i++){
                sdrSelect[i] ^= sdrSelectBuffer[i];
            }
        }
        for(int i=0; i<sdrCnt; i++){
            sdrSelectBuffer[i] = sdrSelect[i];
        }

        //render background
        ClearBackground(BLACK);
        Graph::setDimensions(GRAPH_PADDING, GRAPH_PADDING+MENU_Y_START+MENU_HEIGHT, sw-GRAPH_PADDING*2, sh-GRAPH_PADDING*2-MENU_HEIGHT-MENU_Y_START, (double) dbTop, (double) dbBottom, freqStart, freqEnd);
        MenuEntry::renderBG();
        Graph::renderGrapBackground();

        
        //render graphs
        for(int i=0; i<sdrCnt; i++){
            graphs[i]->render();
        }
    
        
        //render Menus
        m_Dim.render(mx, my);
        m_Sdr.render(mx, my);
        
        //fps counter
        DrawText(TextFormat("%d FPS", GetFPS()), 500, 5, 20, RED);

        //
        EndDrawing();
    }

    return 0;
}