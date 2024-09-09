#pragma once
#include <vector>
#include <memory>

class SDR{ 
public:
    virtual ~SDR() = default;
    virtual const char* getName() const = 0;
    virtual bool isInited() const = 0;

    static std::vector<std::unique_ptr<SDR>>* getSDRs();
    static void updateSDRs();

private:
    static std::vector<std::unique_ptr<SDR>> sdrs;

};