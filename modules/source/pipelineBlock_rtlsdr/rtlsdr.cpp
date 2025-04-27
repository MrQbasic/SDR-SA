#include "mod-lib/pipelineBlock.hpp"

#include <vector>
#include <rtl-sdr.h>


class pipelineBlock_rtlsdr : public pipelineBlock{
public:
    pipelineBlock_rtlsdr(){
        updateList();
        this->headline = "RTL-SDR";
    }

    ~pipelineBlock_rtlsdr(){
        std::cout << "Fuck we dont have a clean up fuction. Need to implement that!" << std::endl;
    }

    void* getBuffer(int index) override{
        switch(index){
            case 0:
                return &this->centerFrequency;
            default:
                return nullptr;
        }
    }

    void run() override{
        //fetch inputs
        this->centerFrequency = *((double*) this->sourceBuffers[0]);
        //process outputs
    
    }

    PinType getPinType(int id) override{
        switch(id){
            case 0:
                return PinType_value;
            case 50:
                return PinType_spectrum;
            default:
                return PinType_none;
        }
    }

    std::string getModuleName() override {return "pipelineblock_rtlsdr";}

    bool render() override {
        generic_renderStart();

        ImGui::SetNextItemWidth(200);
        if(ImGui::BeginCombo("Device", currentListIndex == -1 ? "please select an SDR" : deviceList[currentListIndex])){
            for(int i=0; i<deviceList.size(); i++){
                if(ImGui::Selectable(deviceList[i])){
                    if(initDevice(i)){
                        currentListIndex = i;
                    }
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Text("%f", centerFrequency);

        setNodeStyle(PinType_value);
        ImNodes::BeginInputAttribute(this->nodeID * 100 + 0);
        ImGui::Text("centerFreq");
        ImNodes::EndInputAttribute();
        ImNodes::PopColorStyle();

        setNodeStyle(PinType_spectrum);
        ImNodes::BeginOutputAttribute(this->nodeID * 100 + 50);
        ImGui::Text("FFT");
        ImNodes::EndOutputAttribute();
        ImNodes::PopColorStyle();

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
        rtlsdr_dev* testSdr;
        for(int i=0; i<count; i++){
            int error = rtlsdr_open(&testSdr, i);
            if(error != 0) continue;
            rtlsdr_close(testSdr);
            //save the device name
            deviceList.push_back(rtlsdr_get_device_name(i));
        }
    }

    rtlsdr_dev* sdr;
    bool initDevice(int listIndex){
        std::cout << "init RTLSDR" << std::endl;
        int error = rtlsdr_open(&sdr, listIndex);
        error |= rtlsdr_set_testmode(sdr, 0);            //make sure we dont get the demo counter
        error |= rtlsdr_set_direct_sampling(sdr, 0);     //disable directl sampling
        error |= rtlsdr_set_sample_rate(sdr, 2400000);   //2.4 Msps
        error |= rtlsdr_reset_buffer(sdr);
        if(error == 0) return true;
        std::cout << "Error: " << error << " during init of RTLSDR!" << std::endl; 
        return false;
    }
    bool buffersInit();


    double centerFrequency;
};


extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_rtlsdr();
}