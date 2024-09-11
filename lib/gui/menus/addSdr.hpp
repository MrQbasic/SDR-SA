#include <source/sdr.hpp>

#define refreshTime 5.0f
float refreshProgress = -1.0f;

void renderAddSdrMenu(){
    ImGui::Begin("Add SDR", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        /*
        Issue where it calls destructors too often
        //check if we need to refresh
        if(refreshProgress >= 1.0f || refreshProgress < 0){
            SDR::updateSDRs();
            refreshProgress = 0.0f;
        }
        //refresh time bar
        ImVec2 cPos = ImGui::GetCursorPos();
        ImVec2 barSize = ImVec2(300.0f, 25.0f);
        ImGui::ProgressBar(refreshProgress, barSize, "");
        ImVec2 cPosNew = ImGui::GetCursorPos();
        refreshProgress += 1.0f / refreshTime * ImGui::GetIO().DeltaTime;
        //-render text inside bar
        ImGui::SetCursorPos(ImVec2(cPos.x + 10, cPos.y + 5));
        ImGui::TextUnformatted("updating SDRs...");
        ImGui::SetCursorPos(ImVec2(cPosNew.x, cPosNew.y));
        */
        if(ImGui::Button("Refresh") || refreshProgress == -1.0f){
            SDR::updateSDRs();
            refreshProgress = 0;
        }


        //list all available SDRs
        std::vector<SDR*>* s = SDR::getSDRs();
        for(const auto& sdr : *s){
            const char* name = sdr->getName();
            //TODO MAKE IT VISABLE THAT SDRS ARE ALREADY INITED (also if you got that maybe not call init on a button press if they are inited to avoid cpu waste)
            if(ImGui::Button(name)){
                if(!sdr->isInited()){ 
                    sdr->init();
                    std::cout << name << std::endl;
                    Source::addSource((Source*) &(*sdr));
                }
            }
        }
    ImGui::End();
}