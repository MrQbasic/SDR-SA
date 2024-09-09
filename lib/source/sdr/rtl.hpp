#include <source/sdr.hpp>
#include <rtl-sdr.h>


class RTLSDR : public SDR{

public:
    RTLSDR(int id){
        this->id = id;
        this->name = rtlsdr_get_device_name(this->id);
        this->inited = false;
    }

    static std::vector<RTLSDR> getSDRs(){
        std::vector<RTLSDR> rtlsdrs;
        int num = rtlsdr_get_device_count();
        for(int i=0; i<num; i++){
            rtlsdrs.push_back(RTLSDR(i));
        }
        return rtlsdrs;
    }

    bool isInited() const override{
        return inited;
    }

    const char* getName() const override {
        return name;
    }

private:
    const char* name;
    int id;
    bool inited;
};