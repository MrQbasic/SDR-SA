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

#include <string>
 
long long freqStart =  80 * 1000 * 1000; //80MHZ
long long freqEnd   = 1700 * 1000 * 1000; //115MHZ
long long dbTop = 10;
long long dbBottom = -100;

bool paused = false;

int main(){
    //init GX
    InitWindow(1280, 720, "SDR - SA");
    SetTargetFPS(60);

    //Gerneal Dimension Settings) spacer = 0
    NumberSelector s_freqStart = NumberSelector(99999999999, 0, &freqStart);
    NumberSelector s_freqEnd   = NumberSelector(99999999999, 0, &freqEnd);
    NumberSelector s_dbTop     = NumberSelector(99, -150, &dbTop);
    NumberSelector s_dbBottom  = NumberSelector(99, -150, &dbBottom);
    Setting* s_Dim[] = {&s_freqStart, &s_freqEnd, &s_dbTop, &s_dbBottom};
    const char* t_Dim[] = {"START:", "END:", "DB TOP:", "DB BOTTOM:"};
    MenuFrame f_Dim = MenuFrame(s_Dim, t_Dim, 4);
    MenuEntry m_Dim = MenuEntry("SPECTRUM", &f_Dim);

    //scan for SDRs
    int sdrCnt = 0;
    const char** sdrNames;
    RTLSDR::getSDRs(&sdrCnt, &sdrNames);

    //main menu
    bool* sdrSelect       = new bool[sdrCnt];
    bool* sdrSelectBuffer = new bool[sdrCnt];
    Setting**    s_Sdr = new Setting*[2*sdrCnt+1];
    const char** t_Sdr = new const char*[2*sdrCnt+1];

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
        Button* sdrButton = new Button(sdrNames[i], &(sdrSelect[i]));
        sdrButton->setupColorText(sdrButtonColors, &(sdrs[i]->state));
        s_Sdr[i*2+1] = sdrButton;
        t_Sdr[i*2+1] = "\n";
        //construct sdr connect menu
        auto sdrPrt = sdrs[i];
        auto conFunction = [sdrPrt, subsubframes](){
            sdrPrt->init();
            subsubframes[1] = new MenuFrame((RTLSDR*)sdrPrt);   // the other menu is created when the init is called
        };
        Setting* connectButton = new Button("Connect",conFunction);
        const char* connectButtonTitle[] = {"\n"};
        //SubSubmenu
        subsubframes[0] = new MenuFrame((&connectButton), connectButtonTitle, 1);
        f_Sdr_submenu[i] = new ConditionalMenuFrame(subsubframes,2,&(sdrs[i]->state));
        s_Sdr[i*2+2] = new Submenu(f_Sdr_submenu[i], &sdrSelect[i]);
        t_Sdr[i*2+2] = "\n";
    }
    

    //SDR selector Menu
    Button s_PauseButton = Button("PAUSE ALL", &paused);
    s_Sdr[0] = &s_PauseButton;
    t_Sdr[0] = "\n";
    MenuFrame* f_Sdr = new MenuFrame(s_Sdr, t_Sdr, 2*sdrCnt+1);
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
            //check if the currently selected SDR is
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