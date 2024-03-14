#pragma once

class SDR{
public:
    int state = 0;
    virtual long long getSampleRate(){return 0;}
    static void getSDRs(int* amount, const char*** names);
    virtual void init(){}
    virtual void disconnect(){}
    //return val set / factor = neg or pos
    virtual int changeGain(int factor) = 0;
    //coppysize = -1 -> all
    virtual void getFFT(double* bufVal, int* bufValCnt, int bufSize, int bufType, long long bufStartFreq, long long bufEndFreq, long long centerFreq){};
};