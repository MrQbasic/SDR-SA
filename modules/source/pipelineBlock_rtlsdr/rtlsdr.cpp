#include "mod-lib/pipelineBlock.hpp"

#include <vector>
#include <rtl-sdr.h>

class pipelineBlock_rtlsdr : public pipelineBlock
{
public:
    pipelineBlock_rtlsdr()
    {
        updateList();
        this->headline = "RTL-SDR";
        // alloc buffers used to store samples
        sampleBuffer = new uint8_t[this->sampleSize];
    }

    ~pipelineBlock_rtlsdr()
    {
        delete[] sampleBuffer;
        std::cout << "Fuck we dont have a clean up fuction. Need to implement that!" << std::endl;
    }

    void *getBuffer(int index) override
    {
        switch (index)
        {
        case 0:
            return (void*) &(this->outputBuffer);
        default:
            return nullptr;
        }
    }

    void run() override
    {
        // fetch inputs
        this->centerFrequency = *((double *)this->sourceBuffers[0]);
        // process outputs
        if (sdr != nullptr)
        {
            updateSamples();
        }
    }

    PinType getPinType(int id) override
    {
        switch (id)
        {
        case 0:
            return PinType_value;
        case 50:
            return PinType_samples;
        default:
            return PinType_none;
        }
    }

    std::string getModuleName() override { return "pipelineblock_rtlsdr"; }

    bool render() override
    {
        generic_renderStart();
        ImGui::SetNextItemWidth(200);
        if (ImGui::BeginCombo("###Device", currentListIndex == -1 ? "please select a SDR" : deviceList[currentListIndex])){
            for (int i = 0; i < deviceList.size(); i++){
                if (ImGui::Selectable(deviceList[i])){
                    if (initDevice(i)){
                        currentListIndex = i;
                    }
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Text("%f", centerFrequency);

        
        setNodeStyle(PinType_value);
        ImNodes::BeginInputAttribute(this->nodeID * 100 + 0);
        ImGui::Text("freq");
        ImNodes::EndInputAttribute();
        ImNodes::PopColorStyle();
        
        renderSpacer("samples", 200);
        
        setNodeStyle(PinType_samples);
        ImNodes::BeginOutputAttribute(this->nodeID * 100 + 50);
        ImGui::Text("samples");
        ImNodes::EndOutputAttribute();
        ImNodes::PopColorStyle();
        
        return generic_renderEnd();
    }

    bool renderAdd() override
    {
        return ImGui::MenuItem("RTL-SDR");
    }

private:
    int currentListIndex = -1;
    std::vector<const char *> deviceList;
    void updateList()
    {
        deviceList.clear();
        int count = rtlsdr_get_device_count();
        rtlsdr_dev *testSdr;
        for (int i = 0; i < count; i++)
        {
            int error = rtlsdr_open(&testSdr, i);
            if (error != 0)
                continue;
            rtlsdr_close(testSdr);
            // save the device name
            deviceList.push_back(rtlsdr_get_device_name(i));
        }
    }

    rtlsdr_dev *sdr = nullptr;
    bool initDevice(int listIndex)
    {
        std::cout << "init RTLSDR" << std::endl;
        int error = rtlsdr_open(&sdr, listIndex);
        error |= rtlsdr_set_testmode(sdr, 0);          // make sure we dont get the demo counter
        error |= rtlsdr_set_direct_sampling(sdr, 0);   // disable directl sampling
        error |= rtlsdr_set_sample_rate(sdr, 2400000); // 2.4 Msps
        error |= rtlsdr_reset_buffer(sdr);
        if (error == 0)
            return true;
        std::cout << "Error: " << error << " during init of RTLSDR!" << std::endl;
        return false;
    }
    bool buffersInit();

    double centerFrequency;
    int sampleSize = 512 * 16 * 32;
    uint8_t *sampleBuffer;
    struct pipeline_buffer_samples outputBuffer;

    void updateSamples()
    {
        rtlsdr_set_center_freq(sdr, (uint32_t)centerFrequency);
        int read;
        rtlsdr_read_sync(sdr, sampleBuffer, sampleSize, &read);
        this->outputBuffer.sampleCount = read;
        this->outputBuffer.centerFreq  = centerFrequency;
        this->outputBuffer.samples     = (void*) sampleBuffer;
    }
};

extern "C" pipelineBlock *moduleCreate_pipelineBlock()
{
    return (pipelineBlock *)new pipelineBlock_rtlsdr();
}