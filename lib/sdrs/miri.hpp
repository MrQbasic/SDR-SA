#pragma once

#include "mirisdr.h"
#include <sdr.hpp>
#include <fftw3.h>

#define MIRI_SAMPLE_RATE 500000

class MIRISDR : public SDR{
public:
    MIRISDR(int index){
        this->devIndex = index;
    }

    void init() override{
        //open dev file
        mirisdr_open(&this->mirisdr, this->devIndex);
        mirisdr_reset(this->mirisdr);
        //setup sr and bit depth
        int e = 0;
        e |= mirisdr_set_hw_flavour(this->mirisdr, MIRISDR_HW_DEFAULT);
        e |= mirisdr_set_sample_rate(this->mirisdr, MIRI_SAMPLE_RATE);
        uint32_t sr = mirisdr_get_sample_rate(this->mirisdr);
        std::cout << "Sample rate: " << sr << std::endl; 
        e |= mirisdr_set_bandwidth(this->mirisdr, MIRI_SAMPLE_RATE);
        e |= mirisdr_set_sample_format(this->mirisdr, "252_S16");
        e |= mirisdr_set_transfer(this->mirisdr, "ISOC");
        e |= mirisdr_set_if_freq(this->mirisdr, 0);
        e |= mirisdr_reset_buffer (this->mirisdr);
        std::cout << e << std::endl;
        //alloc buffers
        SampleBuffer = (int16_t*) malloc(sizeof(int16_t) * SampleBufferSize);
        SamplesInput  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SampleBufferSize / 2);
        SamplesOutput = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SampleBufferSize / 2);
        fftplan = fftw_plan_dft_1d(SampleBufferSize/2, SamplesInput, SamplesOutput, FFTW_FORWARD, FFTW_ESTIMATE);
        //mark as init
        std::cout << "SDR INIT OK!" << std::endl;
        this->state = 1;
    }


    static void getSDRs(int* anmount, const char*** names){
        int num = mirisdr_get_device_count();
        (*names) = new const char*[num];
        for(int i=0; i<num; i++){
            (*names)[i] = mirisdr_get_device_name(i);
        }
        *anmount += num;
    }
    
    long long getSampleRate() override {
        return MIRI_SAMPLE_RATE;
    }


    int changeGain(int factor) override{
        return 0;
    }

    void getFFT(double* bufVal, int* bufValCnt, int bufSize, int bufType, long long bufStartFreq, long long bufEndFreq, long long centerFreq) override{
        mirisdr_set_center_freq(this->mirisdr, centerFreq);
        int read = 0;
        int error = mirisdr_read_sync(this->mirisdr, SampleBuffer, SampleBufferSize, &read);

        std::cout << read << " " << error << std::endl;

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
            double db = (10 * log10(magnitude)); 
            //std::cout << magnitude << std::endl;
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

public:
    int devIndex;
    mirisdr_dev_t* mirisdr;

    int16_t* SampleBuffer;
    int      SampleBufferSize = (16 * 16384);

    fftw_complex* SamplesOutput;
    fftw_complex* SamplesInput;
    fftw_plan fftplan;
}; 