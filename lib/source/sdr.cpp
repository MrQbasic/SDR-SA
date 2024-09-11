#include <source/sdr.hpp>
#include <iostream>

#include <source/sdr/rtl.hpp>

std::vector<SDR*> SDR::sdrs;


std::vector<SDR*>* SDR::getSDRs(){ return &sdrs; }

void SDR::updateSDRs(){
    //cleaning unused once
    for(int i=sdrs.size()-1; i>=0; i--){
        auto sdr = sdrs[i];
        if(!sdr->isInited()){
            delete sdr;
            sdrs.erase(sdrs.begin() + i);
        }
    }

    //adding RTLSDRs
    std::vector<RTLSDR*> rtls = RTLSDR::getSDRs();
    for(auto& rtl : rtls){
        if(rtl->alreadyUsed()) continue;
        sdrs.push_back(rtl);
    }

}