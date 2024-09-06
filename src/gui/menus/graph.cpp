#include <gui/menus/graph.hpp>
#include <gui/settings/button.hpp>

#include <iostream>

Menu_Graph::Menu_Graph() : Menu("Graphs", &this->frame){
    this->frame = nullptr;
    updateFrame();
};

void Menu_Graph::updateFrame(){
    //check if we need to do some cleanup
    for(int i=0; i<this->settings.size(); i++){
        delete this->settings[i];
        this->settings.pop_back();
    }
    if(this->frame != nullptr){
        delete this->frame;
    }
    //construct frames
    auto addFunc = [this](){
        this->addGraph();
    };
    this->settings.push_back(new Button("New Graph", addFunc, nullptr, "\n"));
    this->frame = new MenuFrame(this->settings);
}

void Menu_Graph::addGraph(){
    std::cout << "Adding a new Graph" << std::endl;
    updateFrame();
};

std::vector<Graph*>* Menu_Graph::getGraphs(){
    return &this->graphs;
}