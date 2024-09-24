#pragma once

#include <vector>
#include <memory>

class Source{
public:
    //stores as Double array: {min, max}
    double* getFreqRange();

    virtual double getBandwidth() = 0;
    
    //return: count   WARNING: NOT BUFFERD TO PRESERVE MEMORY WATCH OUT WHAT YOU DO WITH DATA
    //passing nulptr is ok
    int getData(double** dataX, double** dataY);
    virtual void updateData(long long centerFreq) = 0;

    virtual const char* getName() const = 0;

    virtual void renderMenu(bool toggle);

    //interface for source listing
    static std::vector<Source*>* getSources();
    static void addSource(Source* source);
    static bool removeSource(Source* source);


    ~Source();

protected:
    double freqHigh, freqLow;
    //TODO: we need buffer swapping
    double* dataX;
    double* dataY;
    int sampleCount;

private:
    static std::vector<Source*> sources;
};