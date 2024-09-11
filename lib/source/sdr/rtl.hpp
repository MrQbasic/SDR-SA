#include <source/sdr.hpp>
#include <rtl-sdr.h>
#include <format>
#include <ImGui/imgui.h>


class RTLSDR : public SDR{

public:
    RTLSDR(int id){
        this->id = id;
        this->inited = false;
        //format name
        int nameLen = std::snprintf(nullptr, 0, "%s #%d   ", rtlsdr_get_device_name(this->id), id);
        this->name = (const char*) malloc(sizeof(const char) * nameLen);
        std::snprintf((char*)this->name, nameLen, "%s #%d   ", rtlsdr_get_device_name(this->id), id);
    }

    bool alreadyUsed() {
        int error = rtlsdr_open(&this->rtlsdr, this->id);
        if(error != 0) return true;
        rtlsdr_close(this->rtlsdr);
        this->rtlsdr = nullptr;
        return false;
    }

    void renderMenu() override{
        //Remove Button
        if(ImGui::MenuItem("Remove")){
            //this->~Source();      // view TODO
            this->inited = false;
            Source::removeSource(this);
            if(this->rtlsdr != 0){
                rtlsdr_close(this->rtlsdr);
            }else{
                std::cout << "unexpected ERROR!" << std::endl;
            }
            SDR::updateSDRs();
            return;
        }
    }

    ~RTLSDR(){
        std::cout << "Who tf calls the RTL DESTRUCTOR ?" << std::endl;
        //rtlsdr_close(this->rtlsdr);
        //free((char*)name);
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

    int init() override{
        this->inited = true;
        rtlsdr_open(&this->rtlsdr, this->id);
        return 0;
    }


private:
    const char* name;
    int id;
    bool inited;

    rtlsdr_dev_t* rtlsdr;
};