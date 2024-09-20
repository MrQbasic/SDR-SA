#include <source/sdr.hpp>
#include <rtl-sdr.h>
#include <format>
#include <ImGui/imgui.h>
#include <fftw3.h>
#include <math.h>

class RTLSDR : public SDR{

public:
    RTLSDR(int id){
        this->id = id;
        this->inited = false;
        //format name
        int nameLen = std::snprintf(nullptr, 0, "%s #%d   ", rtlsdr_get_device_name(this->id), id);
        this->name = (const char*) malloc(sizeof(const char) * nameLen);
        std::snprintf((char*)this->name, nameLen, "%s #%d   ", rtlsdr_get_device_name(this->id), id);
        //alloc for gain
        gainText = new char[10];
        //predefined
        this->sampleCount = 2400000;  //2.4 MSps
    }

    bool alreadyUsed() {
        int error = rtlsdr_open(&this->rtlsdr, this->id);
        if(error != 0) return true;
        rtlsdr_close(this->rtlsdr);
        this->rtlsdr = nullptr;
        return false;
    }

    void renderMenu() override{
        //Gain Slider
        sprintf(this->gainText, "%.1f", (float) gainSettingsValues[gain]/10);
        ImGui::SliderInt((const char*) this->gainText, &gain, 0, gainSettings-1, "Gain", ImGuiSliderFlags_None);
        //Remove Button
        if(ImGui::MenuItem("Remove")){
            //this->~Source();      // view TODO
            this->inited = false;
            Source::removeSource(this);
            if(this->rtlsdr != 0){
                rtlsdr_close(this->rtlsdr);
            }else{
                std::cout << "unexpected ERROR!" << std::endl;
            }
            SDR::updateSDRs();
            return;
        }
    }

    ~RTLSDR(){
        std::cout << "RTL DESTRUCTOR ?" << std::endl;
        delete gainText;
        delete this->data;

        fftw_destroy_plan(fftwPlan);
        fftw_free(fftwInpBuffer);
        fftw_free(fftwOutBuffer);


        //rtlsdr_close(this->rtlsdr);
        //free((char*)name);
    }

    static std::vector<RTLSDR*> getSDRs(){
        std::vector<RTLSDR*> rtlsdrs;
        int num = rtlsdr_get_device_count();
        for(int i=0; i<num; i++){
            rtlsdrs.push_back(new RTLSDR(i));
        }
        return rtlsdrs;
    }

    bool isInited() const override{
        return inited;
    }

    const char* getName() const override {
        return name;
    }

    int init() override{
        //open dev file
        this->inited = true;
        rtlsdr_open(&this->rtlsdr, this->id);
        //get Gain settings
        this->gainSettings = rtlsdr_get_tuner_gains(this->rtlsdr, nullptr);
        this->gainSettingsValues = new int[this->gainSettings];
        rtlsdr_get_tuner_gains(this->rtlsdr, this->gainSettingsValues);
        this->gain = 0;
        //set settings
        rtlsdr_set_testmode(this->rtlsdr, 0);   //disable testmode
        rtlsdr_set_tuner_bandwidth(this->rtlsdr, 0);   //set to auto
        rtlsdr_set_sample_rate(this->rtlsdr, this->sampleCount);  //sample rate = 2.4M
        rtlsdr_set_offset_tuning(this->rtlsdr, 1); // enable offset tuning
        rtlsdr_reset_buffer(this->rtlsdr);
        //alloc buffers
        this->data = new double[this->sampleCount];
        fftwInpBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->sampleCount / 2);
        fftwOutBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->sampleCount / 2);
        fftwPlan = fftw_plan_dft_1d(this->sampleCount / 2, fftwInpBuffer, fftwOutBuffer, FFTW_FORWARD, FFTW_ESTIMATE);

        updateData(0);
        return 0;
    }

    

    void updateData(long long centerFreq) override{
        std::cout << "asd" << std::endl;
        for(int i=0; i<= this->sampleCount; i++){
            data[i] = i % 10;
        }
    }


private:
    //Index to use with gainSettingsValues
    char* gainText;
    int   gain;
    int   gainSettings;
    int*  gainSettingsValues;

    //needed for fftw calculation
    fftw_complex* fftwInpBuffer;
    fftw_complex* fftwOutBuffer;
    fftw_plan     fftwPlan;

    const char* name;
    int id;
    bool inited;

    rtlsdr_dev_t* rtlsdr;
};