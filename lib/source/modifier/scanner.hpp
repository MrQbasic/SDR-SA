#pragma once

#include <gui/menus/settings.hpp>
#include <graph/graph.hpp>
#include <source/source.hpp>
#include <ImGui/imgui.h>
#include <math.h>
#include <cstdint>
#include <cstring>
#include <format>

#include <iostream>

class Source_Scanner : public Source{
public:
    //name gets delated by the Scanner destructor
    Source_Scanner(Source* src){
        //auto gened name
        char* srcName = (char*) src->getName();
        int nameLen = std::snprintf(nullptr, 0, "Scanner: %s", srcName);
        this->name = new char[nameLen];
        std::snprintf(this->name, nameLen, "Scanner: %s", srcName);
        //create scanner and save it so we dont have to worry about the memory
        this->source = src;
        //standart Setup
        this->newNameBuffer = new char[30];
        memccpy(this->newNameBuffer, this->name, sizeof(char), 30);
        id++;
        this->newSampleCount = 1000; // default value is stored there
        this->sampleCount = 0;
        this->dataX = nullptr;
        this->dataY = nullptr;
        this->dataYAvgCnt = nullptr;
        this->nextFreqHigh = 110000000;
        this->nextFreqLow  =  80000000;
        //autocreate the Graph as well
        if(setting_easyMode){
            Graph* newGraph = new Graph((Source*)this);
            Graph::addGraph(newGraph);
        }
    }


    Source_Scanner(){
        this->name = new char[30];
        this->newNameBuffer = new char[30];
        std::snprintf(this->name, 30, "new Scanner #%d", id);
        memccpy(this->newNameBuffer, this->name, sizeof(char), 30);
        id++;
        this->newSampleCount = 1000; // default value is stored there
        this->sampleCount = 0;
        this->dataX = nullptr;
        this->dataY = nullptr;
        this->dataYAvgCnt = nullptr;
        this->nextFreqHigh = 110000000;
        this->nextFreqLow  =  80000000;
        this->source = nullptr;
    }
    
    double getBandwidth() override{
        return this->freqHigh - this->freqLow;
    }

    ~Source_Scanner(){
        delete this->name;
        delete this->newNameBuffer;
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
        if(!this->displayMenu) return;
        if(ImGui::Begin(this->name, &(this->displayMenu), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)){
            //Input Name
            if(ImGui::InputText("Name", newNameBuffer, 30, ImGuiInputTextFlags_EnterReturnsTrue)){
                memccpy(this->name, this->newNameBuffer, sizeof(char), 30);
            }
            
            //Input Source
            std::vector<Source*>* sources = Source::getSources();
            const char** names = (const char**) new char* [sources->size()] ;
            int currentSourceIndex = -1;
            int nameCnt = 0;
            for(int i=0; i<(*sources).size(); i++){
                Source* src = (*sources)[i];
                if(src == (Source*) this) continue;   //you cant scan yourself
                if(src == this->source) currentSourceIndex = nameCnt; //get index of selected SDR
                names[nameCnt] = src->getName();
                nameCnt++;  //we need the extra coutner as i includes the scanner itself so that can create an offset
            }
            int newSourceIndex = currentSourceIndex;
            ImGui::ListBox("Source", &newSourceIndex, names, nameCnt, -1);
            //check if we changed the sdr
            if(currentSourceIndex != newSourceIndex){
                std::cout << "New Source Selected" << std::endl;
                std::cout << "current Src Ptr: " << this->source << std::endl;
                std::cout << "new src index: " << newSourceIndex << std::endl;
                std::cout << "this: " << this << std::endl;
                for(int i=0; i<(*sources).size(); i++){
                    Source* src = (*sources)[i];
                    std::cout << "Global src Index: " << i << " Prt of src: " << src << std::endl;
                    if( src == this) continue;
                    newSourceIndex--;
                    if(newSourceIndex == -1){
                        this->source = (*sources)[i];
                    }
                }
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
    Source* source;

    //menu + buffering of inputs
    bool displayMenu;
    static inline int id = 0;
    char* name;
    char* newNameBuffer;
    double nextFreqHigh;
    double nextFreqLow;

    //needed for avg
    int *dataYAvgCnt;

    //output buffer
    int newSampleCount = 1000;

};