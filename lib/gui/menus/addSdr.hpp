#include <source/sdr.hpp>

#define refreshTime 5.0f
float refreshProgress = -1.0f;

void renderAddSdrMenu(){
    if(ImGui::Button("Refresh") || refreshProgress == -1.0f){
        SDR::updateSDRs();
        refreshProgress = 0;
    }

    //list all available SDRs
    std::vector<SDR*>* s = SDR::getSDRs();
    for(const auto& sdr : *s){
        const char* name = sdr->getName();
        bool isInited = sdr->isInited(); 
        if(isInited){
            ImGui::BeginDisabled();
        }
        if(ImGui::Selectable(name, false, ImGuiSelectableFlags_DontClosePopups)){
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
}