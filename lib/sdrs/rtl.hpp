#include <sdr.hpp>
#include <rtl-sdr.h>
#include <stdlib.h>
#include <iostream>
#include <gui/menu.hpp>
#include <fftw3.h>
#include <math.h>

#pragma once

#define RTLSDR_SAMPLE_RATE 2400000 //2.4 MSPs

class RTLSDR : public SDR{
public:
    RTLSDR(int index){
        this->devIndex = index;
    }

    void init() override{
        //open dev file
        rtlsdr_open(&this->rtlsdr, this->devIndex);
        //get gain settings
        GainSettingAmout = rtlsdr_get_tuner_gains(this->rtlsdr, NULL);
        GainSettings     = (int*) malloc(sizeof(int) * GainSettingAmout);
        rtlsdr_get_tuner_gains(this->rtlsdr, GainSettings);
        //set some settings
        rtlsdr_set_testmode(this->rtlsdr, 0);
        rtlsdr_set_tuner_bandwidth(this->rtlsdr, 0);
        rtlsdr_set_sample_rate(rtlsdr,  2400000);
        rtlsdr_set_offset_tuning(rtlsdr, 1);
        rtlsdr_reset_buffer(this->rtlsdr);
        //alloc sample buffer
        SampleBuffer = (uint8_t*) malloc(sizeof(uint8_t) * SampleBufferSize);
        SamplesInput  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SampleBufferSize / 2);
        SamplesOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SampleBufferSize / 2);
        fftplan = fftw_plan_dft_1d(SampleBufferSize/2, SamplesInput, SamplesOutput, FFTW_FORWARD, FFTW_ESTIMATE);
        //mark as init
        std::cout << "SDR INIT OK!" << std::endl;
        this->state = 1;
    }

    ~RTLSDR(){
        std::cout << "WHO CALLED TF DESTRUCTOR ?" << std::endl;
        this->disconnect();
    }

    void disconnect() override{
        fftw_destroy_plan(fftplan);
        fftw_free(SamplesInput);
        fftw_free(SamplesOutput);
        free(SampleBuffer);
        free(GainSettings);
        rtlsdr_close(rtlsdr);
        this->state = 0;
    }

    long long getSampleRate() override{
        return RTLSDR_SAMPLE_RATE;
    }
    
    static void getSDRs (int* amount, const char*** names){
        int num = rtlsdr_get_device_count();
        (*names) = new const char*[num];
        for(int i=0; i<num; i++){
            const char* sdrName = rtlsdr_get_device_name(i);
            (*names)[i] = sdrName;
        }
        *amount += num;
    }

    void getFFT(double* bufVal, int* bufValCnt, int bufSize, int bufType, long long bufStartFreq, long long bufEndFreq, long long centerFreq){
        //set freq
        rtlsdr_set_center_freq(this->rtlsdr, (uint32_t) centerFreq - (uint32_t) getSampleRate());
        //read samples
        int read = 0;
        rtlsdr_read_sync(this->rtlsdr, SampleBuffer, SampleBufferSize, &read);
        //make them workable for the fft calc
        for(int i=0; i<SampleBufferSize/2; i++){
            SamplesInput[i][0] = (double) SampleBuffer[i*2];
            SamplesInput[i][1] = (double) SampleBuffer[i*2+1];
        }
        //run fft
        fftw_execute(fftplan);
        //make it fit into the output buffer
        long long sampleRate = this->getSampleRate();
        int fftSize = SampleBufferSize/2;
        for(int i=0; i<fftSize; i++){
            //calc val for position
            double magnitude = sqrt(SamplesOutput[i][0] * SamplesOutput[i][0] + SamplesOutput[i][1] * SamplesOutput[i][1]);
            double db = (10 * log10(magnitude)) -150; 
            if(i == 0) continue;
            //buffer index
            long long freqRelativeToCenter = ((double)(((i+fftSize/2)%fftSize)-fftSize/2)/(double)(fftSize-1))*sampleRate;
            long long freqOfSampleRelativeToStart = centerFreq + freqRelativeToCenter - bufStartFreq;
            double posRelative = ((double) freqOfSampleRelativeToStart - sampleRate) / (double)(bufEndFreq-bufStartFreq);
            int index = bufSize * posRelative;
            if(index < 0 || index >= bufSize){
                continue;
            }
            //-
            switch(bufType){
                default:
                case 0:
                    if(bufValCnt[index] < 0){
                        bufVal[index] = db;
                        bufValCnt[index] = 1;
                    }else{
                        bufVal[index] += db;
                        bufValCnt[index] ++;
                    }
                    break;
                case 1:
                    if(bufValCnt[index] < 0){
                        bufVal[index] = db;
                        bufValCnt[index] = 1;
                    }else{
                        if(db > bufVal[index]) bufVal[index] = db;
                    }
                    break;
            }
        }
    }

    int changeGain(int factor){
        //find current gain index
        int gain = rtlsdr_get_tuner_gain(this->rtlsdr);
        int gainIndex = -1;
        for(int i=0; i<GainSettingAmout; i++){
            if(GainSettings[i] == gain){
                gainIndex = i;
                break;
            }
        }
        std::cout << "current gain and gainIndex is: " << gain << " " << gainIndex << std::endl;
        //move gainIndex and cap it
        gainIndex += factor;
        if(gainIndex < 0) gainIndex = 0;
        if(gainIndex > GainSettingAmout-1) gainIndex = GainSettingAmout-1;
        //check if anything changed
        if(gain != GainSettings[gainIndex]){
            int error = -1;
            while(error != 0){
                error = rtlsdr_set_tuner_gain(this->rtlsdr, GainSettings[gainIndex]);
            }
        }
        //return selected gain val
        return(GainSettings[gainIndex]);
    }

private:
    void dataCallback(){

    }

private:
    int devIndex;
    rtlsdr_dev_t* rtlsdr;
    
    int  GainSettingAmout;
    int* GainSettings;

    uint8_t* SampleBuffer;
    int      SampleBufferSize = 512 * 16 * 32;

    fftw_complex* SamplesOutput;
    fftw_complex* SamplesInput;
    fftw_plan fftplan;
};