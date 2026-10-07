#include <iostream>
#include <format>
#include <source/sdr.hpp>
#include <libbladeRF.h>
#include <memory.h>

class BLADERF : public SDR {

public:
    BLADERF(bladerf_devinfo devinfo){
        //copy the dev info
        memcpy(&this->blade_info, &devinfo, sizeof(bladerf_devinfo));
    
        //create the device name
        this->name = new char(33);
        std::snprintf(this->name, 33, "%s", this->blade_info.product);

        this->inited = false;
    }

    int init() override{
        //open the device
        int error = bladerf_open_with_devinfo(&this->blade, &(this->blade_info));
        if(!error){
            std::cout << "Error: can't open bladeRF" << std::endl;
        }
        //check the gain range for every chanel
        channel_count_rx = bladerf_get_channel_count(this->blade, BLADERF_RX);
        //check all the channels for gain ?
        for(int i=0; i<=channel_count_rx; i++){
            
        }
        //everything is ok
        inited = true;
        return 0;
    }

    double getBandwidth() override{
        return 0;
    }

    void renderMenu(bool toggle) override{
        if(toggle){
            this->displayMenu ^= true;
            return;
        }
        if(!this->displayMenu) return;
        if(ImGui::Begin(this->name, &(this->displayMenu), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)){
            ImGui::Text("SN: %s", this->blade_info.serial);
            ImGui::SliderInt("Gain", &gain, -100, 100);

            ImGui::End();
        }
    }

    static void updateSdrList(){
        bladerf_devinfo* devs;
        int count = bladerf_get_device_list(&devs);
        for(int i=0; i<count; i++){
            bladerf* blade;
            int error = bladerf_open_with_devinfo(&blade, &(devs[i]));
            if(error == 0){
                bladerf_close(blade);
                sdrs.push_back(new BLADERF(devs[i]));
            }
        }
    }

    bool isInited() const override{
        return inited;
    }

    const char* getName() const override {
        return name;
    }

    void updateData(long long centerFreq) override{

    }


private:
    bool displayMenu = false;

    int gain;
    size_t channel_count_rx;

    char* name;
    bool inited;

    struct bladerf *blade;
    struct bladerf_devinfo blade_info;
};