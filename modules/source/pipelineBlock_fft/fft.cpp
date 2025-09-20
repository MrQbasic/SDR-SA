#include "mod-lib/pipelineBlock.hpp"
#include <fftw3.h>


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
        struct pipeline_buffer_samples sampleSource = *((struct pipeline_buffer_samples*) this->sourceBuffers[0]);
        if(this->currentSampleCount != sampleSource.sampleCount){
            std::cout << "alright running an update!" << std::endl;
            this->setupFFT(sampleSource.sampleCount); //inits for a given sample count
        }


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
    fftw_complex* fftwInpBuffer;
    fftw_complex* fftwOutBuffer;
    fftw_plan fftwPlan;

    int currentSampleCount = 0;

    bool setupFFT(int sampleCount){
        fftwInpBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sampleCount);
        fftwOutBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sampleCount);
        fftwPlan = fftw_plan_dft_1d(sampleCount, fftwInpBuffer, fftwOutBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
        this->currentSampleCount = sampleCount;
    }


};

extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_fft();
}