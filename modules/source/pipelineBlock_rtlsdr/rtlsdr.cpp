#include "mod-lib/pipelineBlock.hpp"

#include <vector>
#include <rtl-sdr.h>


class piplineBlock_rtlsdr : public pipelineBlock{
public:
    piplineBlock_rtlsdr(){
        updateList();
        this->headline = "RTL-SDR";
    }

    std::string getModuleName() override {return "pipelineblock_rtlsdr";}

    bool render() override {
        generic_renderStart();

        if(ImGui::BeginCombo("Device", currentListIndex == -1 ? "please select an SDR" : deviceList[currentListIndex])){
            for(int i=0; i<deviceList.size(); i++){
                ImGui::Selectable(deviceList[i]);
            }
            ImGui::EndCombo();
        }
        return generic_renderEnd();
    }

    bool renderAdd() override{
        return ImGui::MenuItem("RTL-SDR");
    }

private:
    int currentListIndex = -1;
    std::vector<const char *> deviceList;
    void updateList(){
        deviceList.clear();
        int count = rtlsdr_get_device_count();
        for(int i=0; i<count; i++){
            deviceList.push_back(rtlsdr_get_device_name(i));
        }
    }
};


extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new piplineBlock_rtlsdr();
}