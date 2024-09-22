#include <source/sdr.hpp>

#define refreshTime 5.0f
float refreshProgress = -1.0f;

void renderAddSdrMenu(bool* display){
    ImGui::Begin("Add SDR", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        
        if(ImGui::Button("Refresh") || refreshProgress == -1.0f){
            SDR::updateSDRs();
            refreshProgress = 0;
        }

        ImGui::SameLine();
        if(ImGui::Button("Close")){
            *display = false;
        }


        //list all available SDRs
        std::vector<SDR*>* s = SDR::getSDRs();
        for(const auto& sdr : *s){
            const char* name = sdr->getName();
            bool isInited = sdr->isInited(); 
            if(isInited){
                ImGui::BeginDisabled();
            }
            if(ImGui::Button(name)){
                if(!sdr->isInited()){ 
                    sdr->init();
                    std::cout << name << std::endl;
                    Source::addSource((Source*) &(*sdr));
                }
            }
            if(isInited){
                ImGui::EndDisabled();
            }
        }
    ImGui::End();
}