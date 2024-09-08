#include <scanner.hpp>
#include <iostream>
#include <math.h>

Scanner::Scanner(SDR* sdr, double** bufferVal, int** bufferValCnt, int graphType, int size, long long start, long long end){
    this->sdr = sdr; 
    this->graphType          = graphType;
    this->outputBufferSize = size;
    this->outputBufferVal = bufferVal;
    this->outputBufferValCnt = bufferValCnt;
    this->start = start;
    this->end   = end;
    //init buffers
    *this->outputBufferVal    = new double[size];
    *this->outputBufferValCnt = new int[size];
    for(int i=0; i<size; i++){
        (*this->outputBufferVal)[i] = 0;
        (*this->outputBufferValCnt)[i] = 1;
    }
    //bufferState = (bufferState+1)%bufferCnt;
    //start thread
    this->run = true;
    this->died = false;
    this->thread = new std::thread(&Scanner::execute, this);
}

void Scanner::execute(){
    long long sampleRate = this->sdr->getSampleRate();      // = to section bandwitdh
    while(run){
        //signal that the cnt is old (wont change render)
        for(int i=0; i<this->outputBufferSize; i++){
            (*this->outputBufferValCnt)[i] *= -1;
        }
        //do sampling
        long long currentCenterFreq = this->start + sampleRate/2;
        while(currentCenterFreq < this->end+sampleRate*2){
            if(!run) break;
            //get samples
            this->sdr->getFFT(*outputBufferVal, *outputBufferValCnt, outputBufferSize, graphType, start, end, currentCenterFreq);
            currentCenterFreq += sampleRate;
        }
    }
    this->died = true;
}

void Scanner::stop(){
    if(!this->run){
        std::cout << "Why are you trying to kill a not running thread ?!?" << std::endl;
        return;
    }
    this->run = false;
    while(!this->died);
}

Scanner::~Scanner(){
    this->stop();
    this->thread->join();
    delete this->thread;
}