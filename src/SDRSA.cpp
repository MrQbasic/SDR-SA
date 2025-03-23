#include <iostream>

#include <gui/setup.hpp>
#include <gui/gui.hpp>
#include <pipeline/pipeline.hpp>

//default values 
long long freqStart =  85 * 1000 * 1000; //80MHZ
long long freqEnd   = 115 * 1000 * 1000; //115MHZ
long long dbTop = -30;
long long dbBottom = -100;


int main(){
    //setup window
    GLFWwindow* window = setupWindow();
    if(!window) return 0;

    ImPlot::CreateContext();

    ImGui::StyleColorsDark();

    //loads the modules
    setupGui();
    setupPipline();

    while(!glfwWindowShouldClose(window)){
        //Presetup
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0){
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Current Setup
        renderGui();

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
    return 0;
}