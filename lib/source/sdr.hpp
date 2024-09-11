#pragma once
#include <source/source.hpp>

class SDR : public Source{ 
public:
    virtual ~SDR() = default;
    virtual bool isInited() const = 0;
    virtual int init() = 0;

    static std::vector<SDR*>* getSDRs();
    static void updateSDRs();

    void pass(){

    };

private:

    static std::vector<SDR*> sdrs;

};