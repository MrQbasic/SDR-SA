#include <gui/setting.hpp>
#include <gui/settings/button.hpp>
#include <gui/settings/text.hpp>

#include <sdr.hpp>
#include <sdrs/rtl.hpp>
#include <sdrs/miri.hpp>

class Menu_SDR{
public:
    MenuEntry* getMenuEntry() { 
        return mEntry;
    }
    void updateSDRs(){
        //check sdrs
        int sdrCount = 0;
        this->sdrCntRtl = 0;
        RTLSDR::getSDRs(&this->sdrCntRtl, &this->sdrNamesRtl);
        sdrCount += this->sdrCntRtl;
        this->sdrCntMiri = 0;
        MIRISDR::getSDRs(&this->sdrCntMiri, &this->sdrNamesMiri);
        sdrCount += this->sdrCntMiri;
        //check if there are sdrs present!
        if(sdrCount == 0){
            //construct Settings
            std::vector<Setting*> noSdrSetting;
            noSdrSetting.push_back(new Text("No SDR Present!", "\n"));
            noSdrSetting.push_back(new Button("Refresh", nullptr, nullptr, "\n"));
            this->mFrame = new MenuFrame(noSdrSetting);
            this->mEntry = new MenuEntry("SDR", this->mFrame);
            return;
        }

        return;
    }

private:
    //buffer all connected SDRs
    int sdrCntRtl;
    const char** sdrNamesRtl = nullptr;
    int sdrCntMiri;
    const char** sdrNamesMiri = nullptr;
    //Menu objects
    MenuEntry* mEntry = nullptr;
    MenuFrame* mFrame = nullptr;
};