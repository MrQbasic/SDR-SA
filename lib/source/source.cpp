#include <source/source.hpp>
#include <ImGui/imgui.h>
#include <algorithm>
#include <iostream>


std::vector<Source*>  Source::sources;

long long* Source::getFreqRange(){
    return new long long[2]{this->freqHigh, this->freqLow};
}

int Source::getData(double** dataPtr){
    (*dataPtr) = this->data;
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


void Source::renderMenu(){
    if(ImGui::MenuItem("Remove")){
        //this->~Source();      // view TODO
        Source::removeSource(this);
        return;
    }
}