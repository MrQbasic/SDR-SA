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
            //list all the sources in use
            
            if(ImGui::MenuItem("+ SDR")){
                display_menuAddSdr ^= true;
            }
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();


    //windows submenues
    if(display_menuAddSdr){
        ImGui::Begin("Add SDR", nullptr, ImGuiWindowFlags_NoCollapse);
        std::vector<std::unique_ptr<SDR>>* s = SDR::getSDRs();
        if(s->empty()){
            SDR::updateSDRs();
            s = SDR::getSDRs();
        }
        for(const auto& sdr : *s){
            const char* name = sdr->getName();
            if(ImGui::Button(name)){
                std::cout << name << std::endl;
            }

        }

        ImGui::End();
    }
}