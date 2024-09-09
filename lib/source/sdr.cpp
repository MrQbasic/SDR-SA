#include <source/sdr.hpp>
#include <iostream>

#include <source/sdr/rtl.hpp>

std::vector<std::unique_ptr<SDR>> SDR::sdrs;


std::vector<std::unique_ptr<SDR>>* SDR::getSDRs(){ return &sdrs; }

void SDR::updateSDRs(){
    //adding RTLSDRs
    std::vector<RTLSDR> rtls = RTLSDR::getSDRs();

    for(auto& rtl : rtls){
        sdrs.push_back(std::make_unique<RTLSDR>(rtl));
    }

}