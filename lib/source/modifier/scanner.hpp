#include <source/source.hpp>
#include <ImGui/imgui.h>
#include <math.h>
#include <cstdint>
#include <cstring>
#include <format>

class Source_Scanner : public Source{
public:
    Source_Scanner(){
        this->sourceIndex = -1;
        this->name = new char[30];
        this->titleName = new char[30];
        std::snprintf(this->name, 30, "new Scanner #%d", id);
        std::snprintf(this->titleName, 30, "new Scanner #%d", id);
        id++;
        this->newSampleCount = 1000; // default value is stored there
        this->sampleCount = 0;
        this->dataX = nullptr;
        this->dataY = nullptr;
        this->dataYAvgCnt = nullptr;
        this->nextFreqHigh = 100000000;
        this->nextFreqLow  =  90000000;
        this->source = nullptr;
    }
    
    double getBandwidth() override{
        return this->freqHigh - this->freqLow;
    }

    ~Source_Scanner(){
        delete this->name;
        delete this->titleName;
        delete this->dataX;
        delete this->dataY;
        delete this->dataYAvgCnt;
    }

    const char* getName() const override {
        return (const char*) name;
    }

    void updateData(long long centerFreq) override {
        //resize output Buffer is needed
        if(this->sampleCount != this->newSampleCount ||
           this->nextFreqHigh != this->freqHigh ||
           this->nextFreqLow  != this->freqLow){
            std::cout << "Update of scanner called!" << std::endl;
            //reset outputBuffers
            if(this->dataX != nullptr) delete this->dataX;
            if(this->dataY != nullptr) delete this->dataY;
            if(this->dataYAvgCnt != nullptr) delete this->dataYAvgCnt;
            this->dataX = nullptr; this->dataY = nullptr;
            //create new once
            this->sampleCount = this->newSampleCount;
            this->dataX = new double[sampleCount];
            this->dataY = new double[sampleCount];
            this->dataYAvgCnt = new int[sampleCount];
            //copy over freq limits
            this->freqHigh = this->nextFreqHigh;
            this->freqLow  = this->nextFreqLow;
            //populate buffers with starting values
            double freqWidth = this->freqHigh - this->freqLow;
            double freqStep  = freqWidth / sampleCount;
            std::cout << "feqStep in Hz " << freqStep; 
            for(int i=0; i<sampleCount; i++){
                this->dataX[i] = (i * freqStep) + freqLow;
                this->dataY[i] = 0;
            }

        }
        //check if we even have a source
        if(this->source == nullptr){
            return;
        }
        try{
            //centerFreq start val calc
            double srcBw = this->source->getBandwidth();
            double currentCenterFreq = this->freqLow + srcBw/2;
            //go though all sweep pos
            bool done = false;
            int lastIndex = -1;
            while(!done){
                //get samples
                this->source->updateData(currentCenterFreq);
                double *srcDataX, *srcDataY;
                int cnt = this->source->getData(&srcDataX, &srcDataY);
                //downsize them
                for(int i=0; i<cnt; i++){
                    //y mapping calculation
                    double xVal =  srcDataX[i];
                    if(xVal > this->freqHigh){
                        done = true;
                        break;
                    }
                    double freqOffset = xVal - freqLow;
                    double bw = getBandwidth();
                    int index = (int) ((freqOffset/bw)*sampleCount);
                    if(index >= sampleCount){
                        std::cout << "unexpected Error";
                        done = true;
                        break;
                    }
                    if(lastIndex != index){
                        dataY[index] = 0;
                        dataYAvgCnt[index] = 0;
                        if(lastIndex != -1){
                            dataY[lastIndex] /= dataYAvgCnt[lastIndex];
                        } 
                        lastIndex = index;
                    }
                    dataY[index] += srcDataY[i];
                    dataYAvgCnt[index] ++;
                }

                currentCenterFreq += srcBw;
            }
        }catch(std::exception e){
            std::cout << "exception cought. Continuing!" << std::endl;
        }
    }

    void renderMenu(bool toggle) override {
        if(toggle){
            this->displayMenu ^= true;
            return;
        }
        if(!this->displayMenu){
            std::memcpy(this->titleName, this->name, sizeof(char)*30);
            return;
        }
        if(ImGui::Begin(this->titleName, &(this->displayMenu), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)){
            //Input Name
            ImGui::InputText("Name", name, 30, ImGuiInputTextFlags_None);
            //Input Source
            //TODO:: Working with source index is shit as they change switch to using it or ptrs to identify it
            std::vector<Source*>* sources = Source::getSources();
            const char** names = (const char**) new char* [sources->size()] ;
            int cnt = 0;
            for(auto src: *sources){
                if(src == this) continue;
                names[cnt] = src->getName();
                cnt++;
            }
            ImGui::ListBox("Source", &this->sourceIndex, names, cnt, -1);
            if(sourceIndex != -1){
                this->source = (*sources)[this->sourceIndex];
            }
            //Input OutBuffer Size
            if(ImGui::InputInt("Output Size", &(this->newSampleCount))){
                //limit the size 
                if(this->newSampleCount < 1000) this->newSampleCount = 1000;
                if(this->newSampleCount > 50000) this->newSampleCount = 50000;
            }
            //Input Freq
            auto tmpFreqHigh = this->nextFreqHigh;
            if(ImGui::InputDouble("FreqHigh", &tmpFreqHigh)){
                if(tmpFreqHigh > this->nextFreqLow){
                    this->nextFreqHigh = tmpFreqHigh;
                }
            }
            auto tmpFreqLow = this->nextFreqLow;
            if(ImGui::InputDouble("FreqLow", &tmpFreqLow)){
                if(tmpFreqLow < this->nextFreqHigh){
                    this->nextFreqLow = tmpFreqLow;
                }
            }
            //TODOInput Downsample Type
            
            //Input Remove
            if(ImGui::Button("Remove")){
                //remove for list
                Source::removeSource(this);
                //TODO Dinit
            }   
            ImGui::End();
        }
    }

    
private:
    //source
    int sourceIndex;
    Source* source;

    //menu + buffering of inputs
    bool displayMenu;
    static int id;
    char* name;
    char* titleName;
    double nextFreqHigh;
    double nextFreqLow;

    //needed for avg
    int *dataYAvgCnt;

    //output buffer
    int newSampleCount = 1000;

};

int Source_Scanner::id = 0;