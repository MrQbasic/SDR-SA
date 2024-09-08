#include <source/sdr.hpp>

bool display_menuAddSdr = false;

void renderMenuBar(){
    //top Menu
    ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("Graphs")){
            if(ImGui::MenuItem("Hello")){
            
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Sources")){
            if(ImGui::MenuItem("+ SDR")){
                display_menuAddSdr ^= true;
            }
            
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();


    //windows submenues
    if(display_menuAddSdr){
        ImGui::Begin("Add SDR");
        ImGui::End();
    }
}