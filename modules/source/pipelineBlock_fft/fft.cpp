#include "mod-lib/pipelineBlock.hpp"

class pipelineBlock_fft : public pipelineBlock {
public:
    pipelineBlock_fft(){
        this->headline = "FFT";
    }

    ~pipelineBlock_fft(){
        std::cout << "Fuck we dont have a clean up fuction. Need to implement that!" << std::endl;
    }
    
    void* getBuffer(int index) override{
        return nullptr;
    }

    void run() override {
        std::cout << "Yeah needs to be implemented" << std::endl;
    }

    PinType getPinType(int id) override{
        switch(id){
            case 0:
                return PinType_samples;
            case 50:
                return PinType_spectrum;
            default:
                return PinType_none;
        }
    }

    std::string getModuleName() override{return "pipelineblock_fft";}

    bool render() override{
        generic_renderStart();


        setNodeStyle(PinType_samples);
        ImNodes::BeginInputAttribute(this->nodeID * 100 + 0);
        ImGui::TextUnformatted("samples");
        ImNodes::EndInputAttribute();
        ImNodes::PopColorStyle();

        ImGui::SameLine();

        setNodeStyle(PinType_spectrum);
        ImNodes::BeginOutputAttribute(this->nodeID * 100 + 50);
        ImGui::TextUnformatted("spectrum");
        ImNodes::EndOutputAttribute();
        ImNodes::PopColorStyle();

        return generic_renderEnd();
    }

    bool renderAdd() override{
        return ImGui::MenuItem("FFT");
    }

private:

};

extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_fft();
}