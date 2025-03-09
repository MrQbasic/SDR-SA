#include <source/sdr.hpp>
#include <iostream>

#include <source/source.hpp>
#include <source/sdr/rtl.hpp>
#include <source/sdr/bladeRF.hpp>

#include <gui/menus/settings.hpp>

std::vector<SDR*> SDR::sdrs;


std::vector<SDR*>* SDR::getSDRs(){ return &sdrs; }

void SDR::updateSDRs(){
    //clean out all the uninited sdrs
    for(int i=sdrs.size(); i>0; i--){
        if(!(sdrs[0]->isInited())){
            delete sdrs.at(i);
            sdrs.erase(sdrs.begin() + i);
        }
    }
    //populate the list again
    RTLSDR::updateSdrList();
    BLADERF::updateSdrList();
}


void SDR::commonInit(){
    if(setting_easyMode){
        Source_Scanner* sdrScanner = new Source_Scanner((Source*)this);
        Source::addSource(sdrScanner);
    }
}