#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <GLES2/gl2.h>

#include <iostream>

#include <gui/setup.hpp>
#include <gui/menuBar.hpp>

//default values 
long long freqStart =  85 * 1000 * 1000; //80MHZ
long long freqEnd   = 115 * 1000 * 1000; //115MHZ
long long dbTop = -30;
long long dbBottom = -100;


int main(){
    //setup window
    GLFWwindow* window = setupWindow();
    if(!window) return 0;

    ImGui::StyleColorsDark();


    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        renderMenuBar();


        //Rendering of the current frame
        ImGui::Render();
        int sw, sh;
        glfwGetFramebufferSize(window, &sw, &sh);
        glViewport(0,0, sw, sh);
        glClearColor(0.55f, 0.55f, 0.55f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }


    //General Dimension Settings) spacer = 0
    //std::vector<Setting*> settingsDimension;
    //settingsDimension.push_back(new NumberSelector(99999999999, 0,    &freqStart, "START:"));
    //settingsDimension.push_back(new NumberSelector(99999999999, 0,    &freqEnd,   "END:"));
    //settingsDimension.push_back(new NumberSelector(99,          -150, &dbTop,     "DB TOP:"));
    //settingsDimension.push_back(new NumberSelector(99,          -150, &dbBottom,  "DB BOTTOM:"));
    //MenuFrame f_Dim = MenuFrame(settingsDimension);
    //MenuEntry m_Dim = MenuEntry("SPECTRUM", &f_Dim);

    /*
    //General Settings
    std::vector<Setting*> settingsGeneral;
    //-render Cursor
    bool renderCursor = false;
    settingsGeneral.push_back(new Button("CURSOR", nullptr, &renderCursor, "\n"));
    MenuEntry menuGeneral = MenuEntry("SETTING", new MenuFrame(settingsGeneral)); 

    //scan for SDRs
    int sdrCntRtl = 0;
    const char** sdrNamesRtl;
    RTLSDR::getSDRs(&sdrCntRtl, &sdrNamesRtl);
    int sdrCntMiri = 0;
    const char** sdrNamesMiri;
    MIRISDR::getSDRs(&sdrCntMiri, &sdrNamesMiri);

    int sdrCnt = sdrCntMiri + sdrCntRtl;


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
        //ajust index
        int localIndex = i;
        const char** sdrNames;
        if(i < sdrCntRtl){
            sdrs[i] = new RTLSDR(i);
            sdrNames = sdrNamesRtl;
        }else{
            localIndex -= sdrCntRtl;
            sdrs[i] = new MIRISDR(i-sdrCntRtl);
            sdrNames = sdrNamesMiri;
        }
        graphs[i] = new Graph(graphColors[i], sdrs[i]);
        MenuFrame** subsubframes = new MenuFrame*[2];
        //construct SDR selector
        sdrSelect[i] = false;
        sdrSelectBuffer[i] = false;
        Button* sdrButton = new Button(sdrNames[localIndex], nullptr, &(sdrSelect[i]), "\n");
        sdrButton->setupColorText(sdrButtonColors, &(sdrs[i]->state));
        settingsSdr.push_back(sdrButton);
        //construct sdr connect menu
        auto sdrPrt = sdrs[i];
        auto graphPrt = graphs[i];
        auto conFunction = [sdrPrt, graphPrt, subsubframes](){
            sdrPrt->init();
            subsubframes[1] = new MenuFrame(sdrPrt, graphPrt);   // the other menu is created when the init is called
        };
        std::vector<Setting*> connectSettings = {new Button("Connect",conFunction, nullptr, "\n")};
        //SubSubmenu
        subsubframes[0] = new MenuFrame(connectSettings);
        f_Sdr_submenu[i] = new ConditionalMenuFrame(subsubframes,2,&(sdrs[i]->state));
        settingsSdr.push_back(new Submenu(f_Sdr_submenu[i], &sdrSelect[i], "\n"));
    }
    
    */

    //SDR selector Menu
    //bool paused = false;
    //settingsSdr.insert(settingsSdr.begin(), new Button("PAUSE ALL", &paused, "\n"));
    //MenuFrame* f_Sdr = new MenuFrame(settingsSdr);
    //MenuEntry m_Sdr = MenuEntry("SDR", f_Sdr);

    //Menu_SDR menuSDR = Menu_SDR();
    //menuSDR.updateSDRs();

    //m_Sdr = *menuSDR.getMenuEntry();

    
    //calculate position of submenus
    //int sdrMenuPosX = f_Sdr->getPosX();
    //for(int i=0; i<sdrCnt; i++){
    //    if( sdrMenuPosX > GetScreenWidth()/2){
    //        f_Sdr_submenu[i]->setPosX(sdrMenuPosX - f_Sdr_submenu[i]->getWidth());
    //    }else{
    //        f_Sdr_submenu[i]->setPosX(sdrMenuPosX + f_Sdr->getWidth());
    //    }
    //}



    //Menu_Graph* graph = new Menu_Graph();

    //std::vector<Graph*>* graphs;

    /*

    //main render Loop
    while(!WindowShouldClose()){
        BeginDrawing();
        //
        int mx = GetMouseX();
        int my = GetMouseY();
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        
        //check for an action
        if(IsMouseButtonPressed(0)){
            //m_Dim.action(mx, my, 0);
            //m_Sdr.action(mx, my, 0);
            //menuGeneral.action(mx, my, 0);
            graph->action(mx, my, 0);
        }
        if(IsMouseButtonPressed(1)){
            //m_Dim.action(mx, my, 1);
            //m_Sdr.action(mx, my, 1);
            //menuGeneral.action(mx, my, 1);
        }
        //if(IsKeyPressed(KEY_SPACE)){
        //   paused ^= 1;
        //}
        
        //sdrmenu submenu
        //int sdrSubmenuChanges = 0;
        //for(int i=0; i<sdrCnt; i++){
        //    if(sdrSelect[i])sdrSubmenuChanges++;
        //}
        //if(sdrSubmenuChanges > 1){
        //    for(int i=0; i<sdrCnt; i++){
        //        sdrSelect[i] ^= sdrSelectBuffer[i];
        //    }
        //}
        //for(int i=0; i<sdrCnt; i++){
        //    sdrSelectBuffer[i] = sdrSelect[i];
        //}

        //render background
        ClearBackground(BLACK);
        Graph::setDimensions(GRAPH_PADDING, GRAPH_PADDING+MENU_Y_START+MENU_HEIGHT, sw-GRAPH_PADDING*2, sh-GRAPH_PADDING*2-MENU_HEIGHT-MENU_Y_START, (double) dbTop, (double) dbBottom, freqStart, freqEnd);
        Menu::renderHeader();
        Graph::renderGrapBackground();

        graphs = graph->getGraphs();



        //render graphs
        //for(int i=0; i<sdrCnt; i++){
        //    graphs[i]->render();
        //}
        //
        if(true){
            Graph::renderCursor(mx, my);
        }
        
        //render Menus
        //m_Dim.render(mx, my);
        //m_Sdr.render(mx, my);
        //menuGeneral.render(mx, my);
        graph->update();
        graph->render(mx, my);

        //fps counter
        DrawText(TextFormat("%d FPS", GetFPS()), 500, 5, 20, RED);

        //
        EndDrawing();
    }
    */
    return 0;
}