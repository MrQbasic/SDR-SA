#include <source/sdr.hpp>
#include <rtl-sdr.h>
#include <format>
#include <ImGui/imgui.h>
#include <fftw3.h>
#include <math.h>
#include <stdexcept>

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
        this->sampleCount = 1000;  //out defaukt
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
        if(ImGui::SliderInt((const char*) this->gainText, &gain, 0, gainSettings-1, "Gain", ImGuiSliderFlags_None)){
            sprintf(this->gainText, "%.1f", (float) gainSettingsValues[gain]/10);
            rtlsdr_set_tuner_gain(this->rtlsdr, gainSettingsValues[gain]);
        }
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
        delete this->dataX;
        delete this->dataY;
        delete sampleBuffer;

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
        sprintf(this->gainText, "%.1f", (float) gainSettingsValues[gain]/10);
        //set settings
        rtlsdr_set_testmode(this->rtlsdr, 0);   //disable testmode
        rtlsdr_set_tuner_bandwidth(this->rtlsdr, 0);   //set to auto
        rtlsdr_set_sample_rate(this->rtlsdr, 2400000);  //sample rate = 2.4M
        rtlsdr_set_offset_tuning(this->rtlsdr, 1); // enable offset tuning
        rtlsdr_reset_buffer(this->rtlsdr);
        //alloc buffers
        //-out buffer
        this->dataX = new double[this->sampleCount];
        this->dataY = new double[this->sampleCount];
        //-internal buffers
        this->sampleBuffer = new uint8_t[sampleBufferSize];
        fftwInpBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->sampleBufferSize / 2);
        fftwOutBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->sampleBufferSize / 2);
        fftwPlan = fftw_plan_dft_1d(this->sampleBufferSize / 2, fftwInpBuffer, fftwOutBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
        //-check for errors
        if(this->sampleBuffer == nullptr || this->dataX == nullptr || this->dataY == nullptr || this->fftwInpBuffer == nullptr || this->fftwOutBuffer == nullptr || this->fftwPlan == nullptr){
            std::cout << "ERROR: Cant allocate all buffers for RTL-SDR!" << std::endl;
            std::exit(-1);
        }
        //push a first update TODO substitute for proper pipeline system
        updateData(100000000);
        return 0;
    }

    

    void updateData(long long centerFreq) override{
        if(this->inited == false){
            throw -1;
            return;
        }
        //tune to frequency
        rtlsdr_set_center_freq(this->rtlsdr, centerFreq);
        //read from dev
        int read = 0;
        rtlsdr_read_sync(this->rtlsdr, sampleBuffer, sampleBufferSize, &read);
        int fftSize = sampleBufferSize / 2;
        //convert to fftw3 types
        for(int i=0; i<fftSize; i++){
            fftwInpBuffer[i][0] = (double) sampleBuffer[i*2];
            fftwInpBuffer[i][1] = (double) sampleBuffer[i*2+1];
        }
        //run fft
        fftw_execute(this->fftwPlan);
        //compress to make it fit the output buffer
        int outBufPos = 0;
        int inpBufPos = 0;
        double accum_db = 0;
        int    accum_cnt = 0;
        while(inpBufPos <= fftSize){
            //calc db for freqency
            int hwBufPos = (inpBufPos + fftSize/2 ) % fftSize;
            double current_mag = sqrt(fftwOutBuffer[hwBufPos][0] * fftwOutBuffer[hwBufPos][0] + fftwOutBuffer[hwBufPos][1] * fftwOutBuffer[hwBufPos][1]);
            double current_db = (20 * log10(current_mag)) -140; 
            int current_outButPos = (int) (((double)inpBufPos/(double)fftSize) * (double) this->sampleCount);
            if(current_outButPos != outBufPos){
                dataY[outBufPos] = (centerFreq-1200000)+((double)outBufPos/(double)this->sampleCount)*(2400000);
                dataX[outBufPos] = accum_db / accum_cnt;
                accum_cnt = 0;
                accum_db = 0;
                outBufPos = current_outButPos;
            }
            accum_cnt++;
            accum_db += current_db;
            
            inpBufPos++;
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

    uint8_t* sampleBuffer;
    const int sampleBufferSize = 512 * 16 * 32;

    rtlsdr_dev_t* rtlsdr;
};