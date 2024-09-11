#include <source/sdr.hpp>
#include <iostream>

#include <source/sdr/rtl.hpp>

std::vector<std::unique_ptr<SDR>> SDR::sdrs;


std::vector<std::unique_ptr<SDR>>* SDR::getSDRs(){ return &sdrs; }

void SDR::updateSDRs(){
    //cleaning unused once
    for(int i=sdrs.size()-1; i>=0; i--){
        const auto& sdr = sdrs[i];
        if(!sdr->isInited()){
            sdrs.pop_back();
        }
    }

    //adding RTLSDRs
    std::vector<RTLSDR> rtls = RTLSDR::getSDRs();
    for(auto& rtl : rtls){
        if(rtl.alreadyUsed()) continue;
        sdrs.push_back(std::make_unique<RTLSDR>(rtl));
    }

}