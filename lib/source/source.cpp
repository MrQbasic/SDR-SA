#include <source/source.hpp>
#include <ImGui/imgui.h>
#include <algorithm>
#include <iostream>


std::vector<Source*>  Source::sources;

double* Source::getFreqRange(){
    return new double[2]{this->freqHigh, this->freqLow};
    this->sampleCount = -1;
}

int Source::getData(double** dataX, double** dataY){
    
    if(dataX != nullptr) (*dataX) = this->dataX;
    if(dataY != nullptr) (*dataY) = this->dataY;
    return this->sampleCount;
}


std::vector<Source*>* Source::getSources(){
    return &sources;
}
void Source::addSource(Source* source){
    sources.push_back(source);
}
bool Source::removeSource(Source* source){
    int sizeBefore = sources.size();
    sources.erase(std::remove(sources.begin(), sources.end(), source), sources.end());
    return sizeBefore != sources.size();
}

Source::~Source(){
    return;
    
    std::cout << "DESTRUCTOR GETTING CALLED! " << std::endl;

    //remove from list 
    Source::removeSource(this);
    //cleanup
    //delete this->data;
}


void Source::renderMenu(bool toggle){
    if(ImGui::MenuItem("Remove")){
        //this->~Source();      // view TODO
        Source::removeSource(this);
        return;
    }
}