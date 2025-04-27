#pragma once
#include "mod-lib/pipelineBlock.hpp"

#include <memory>
#include <vector>
#include <tuple>
#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <map>
#include <thread>
#include <atomic>

typedef pipelineBlock* (*CreateModuleFunc_pipline)();
std::vector<CreateModuleFunc_pipline> pipelineBlock_createFuncs;
std::vector <std::pair<int, int>> links;
std::map<int, pipelineBlock*> pipelineBlocks;
std::vector<std::unique_ptr<pipelineBlock>> pipelineBlocks_examples;  //dont fucking touch this

void setupPipline(){
    std::string path = "./build/modules/source/";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::string path = entry.path();
            //check if its even the file type we need!
            if(path.find(".so") == std::string::npos) continue;
            if(path.find("pipelineBlock_") == std::string::npos) continue;
            //try loading the module   
            void* handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            if(!handle){
                std::cerr << "Faild to load Module: " << path << std::endl;
                std::cerr <<  dlerror() << std::endl;
                continue;
            }
            dlerror();
            //getting the factory function

            CreateModuleFunc_pipline createFunc = (CreateModuleFunc_pipline) dlsym(handle, "moduleCreate_pipelineBlock");
            if (!createFunc) {
                std::cerr << "Failed to get createModule function: " << dlerror() << "\n";
                dlclose(handle);
                continue;
            }
            pipelineBlock_createFuncs.push_back(createFunc);
            pipelineBlocks_examples.emplace_back(createFunc());
            std::cout << "Module added: " << path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error opening directory: " << e.what() << std::endl;
    }
}



std::vector<int> getNodesBefore(int nodeID){
    std::vector<int> nodeIDs;
    for(int i=0; i<links.size(); i++){
        if(links[i].second / 100 == nodeID) nodeIDs.push_back(links[i].first / 100);
    }
    return nodeIDs;
}

std::vector<int> getNodesAfter(int nodeID){
    std::vector<int> nodeIDs;
    for(int i=0; i<links.size(); i++){
        if(links[i].first / 100 == nodeID) nodeIDs.push_back(links[i].second / 100);
    }
    return nodeIDs;
}



//maping blockID -> position number
std::map<int, int> executionIndex;
void fillNeighbor(int nodeID, int currentIndex){
    //look for an entry with the given key
    auto entry = executionIndex.find(nodeID);
    //check if ther was an entry
    if(entry == executionIndex.end()){
        //for a new entry just write it down
        executionIndex[nodeID] = currentIndex;
    }else{
        //overwrite only when going smaller
        if(currentIndex < entry->second){
            executionIndex[nodeID] = currentIndex;
        }
        if(currentIndex == entry->second) return;
    }
    //get all the connected nodes
    std::vector<int> before = getNodesBefore(nodeID);
    std::vector<int> after  = getNodesAfter(nodeID);
    for(int newNodeID : before){
        fillNeighbor(newNodeID, currentIndex-1);
    }
    for(int newNodeID : after){
        fillNeighbor(newNodeID, currentIndex+1);
    }
}


//thread management
std::thread* dspThread;
std::atomic<bool> shouldRun;
std::atomic<bool> isRunning(false);

int steps;
std::vector<pipelineBlock*>* executionOrder = nullptr;

void runPipeline();

void processPipeline(){
    //stop the processing thread
    shouldRun = false;
    while(isRunning);
    delete dspThread;
    //update the execution Index buffer
    executionIndex.clear();
    //TODO: only works if everything is in one network
    fillNeighbor(pipelineBlocks.begin()->second->getNodeID(), 0);
    //find where to start
    int smallestIndex = 0;
    int biggestIndex = 0;
    for(auto& entry : executionIndex){
        if(entry.second < smallestIndex) smallestIndex = entry.second;
        if(entry.second > biggestIndex)  biggestIndex  = entry.second;
    }
    steps = biggestIndex - smallestIndex + 1;
    //write to the final array
    delete[] executionOrder;
    executionOrder = new std::vector<pipelineBlock*>[steps];
    for(auto& entry : executionIndex){
        auto block = pipelineBlocks.find(entry.first)->second;
        executionOrder[entry.second-smallestIndex].push_back(block);
    }


    /*
    for (const auto& x : executionIndex) {
        std::cout << x.first << ": " << x.second << std::endl;
    }
    */
    shouldRun=true;
    dspThread = new std::thread(runPipeline);
    dspThread->detach();

}

void runPipeline(){
    isRunning=true;
    while(shouldRun){
        for(int step=0; step<steps; step++){
            auto blocks = executionOrder[step];
            for(auto block : blocks){
                block->run();
            }
        }
    }
    isRunning=false;
}