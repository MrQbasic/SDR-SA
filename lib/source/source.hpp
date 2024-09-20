#pragma once

#include <vector>
#include <memory>

class Source{
public:
    //stores as Double array: {min, max}
    long long* getFreqRange();
    
    //return: count   WARNING: NOT BUFFERD TO PRESERVE MEMORY WATCH OUT WHAT YOU DO WITH DATA
    int getData(double** dataPtr);
    virtual void updateData(long long centerFreq) = 0;

    virtual const char* getName() const = 0;

    virtual void renderMenu();

    //interface for source listing
    static std::vector<Source*>* getSources();
    static void addSource(Source* source);
    static bool removeSource(Source* source);


    ~Source();

protected:
    long long freqHigh, freqLow;
    
    double* data;
    int sampleCount;

private:
    static std::vector<Source*> sources;
};