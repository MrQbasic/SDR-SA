
#include <source/source.hpp>

bool display_menuAddSdr = true;

#include <gui/menus/addSdr.hpp>


void renderMenuBar(){
    //top Menu
    ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("Graphs")){
            if(ImGui::MenuItem("Hello")){
            
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Sources")){
            //list all the sources in use
            for(auto src : *Source::getSources()){
                if(ImGui::BeginMenu(src->getName())){
                    
                    src->renderMenu();

                    ImGui::EndMenu();
                }
            }
            //opetions for adding
            if(ImGui::MenuItem("+ SDR")){
                display_menuAddSdr ^= true;
            }
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();


    //windows submenues
    if(display_menuAddSdr){
        renderAddSdrMenu();
    }
}