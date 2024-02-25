#include <scanner.hpp>
#include <math.h>

Scanner::Scanner(SDR* sdr, double* buffer, int size, long long start, long long end){
    this->sdr = sdr; 
    this->outputBuffer = buffer;
    this->outputBufferSize = size;
    this->start = start;
    this->end   = end;
    //start thread
    this->run = true;
    this->died = false;
    this->thread = new std::thread(&Scanner::execute, this);
}

void Scanner::execute(){

    long long bandwidth = this->end - this->start;
    
    int sections = bandwidth / this->sdr->getSampleRate();
    
    double sectionBufferSize = outputBufferSize / sections;
    
    int sizeOfLastSection = outputBufferSize - sectionBufferSize * sections;
    
    if(sizeOfLastSection != 0) sections++;
    
    std::cout << sizeOfLastSection << std::endl;

    while(this->run){
        double bufpos = 0;
        for(int s=0; s<sections; s++){
            long long centerFreq = this->start - this->sdr->getSampleRate() / 2 + this->sdr->getSampleRate()* s;
            if(s == sections-1 && sizeOfLastSection > 0){
                //only do a bit on the last section
                this->sdr->getFFT(this->outputBuffer+(int)round(bufpos), sectionBufferSize, outputBufferSize - bufpos, centerFreq);
                continue;
            }
            this->sdr->getFFT(this->outputBuffer+(int)round(bufpos), sectionBufferSize, -1, centerFreq);
            bufpos += sectionBufferSize; 
        }
    }
    this->died = true;
}

void Scanner::stop(){
    this->run = false;
    while(!this->died);
}