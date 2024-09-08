#include <gui/menus/graph.hpp>
#include <gui/settings/button.hpp>
#include <gui/settings/submenu.hpp>

#include <iostream>

Menu_Graph::Menu_Graph() : Menu("Graphs", &this->frame){
    this->frame = nullptr;
    this->shouldUpdateFrame = true;
    update();
};

void Menu_Graph::update() {
    if(!this->shouldUpdateFrame) return;
    //std::cout << "updateGraph Menu head" << std::endl;
    //check if we need to do some cleanup
    this->settings.clear();
    if(this->frame != nullptr){
        delete this->frame;
    }
    //construct frames
    for(int i=0; i<this->graphs.size(); i++){
        //buffers for subemenu
        std::vector<Setting*> graph_Settings_Submenu_Settings;
        graph_Settings_Submenu_Settings.push_back(new Button("asd", nullptr, nullptr, "\n"));
        MenuFrame* graph_Settings_Submenu_Menu = new MenuFrame(graph_Settings_Submenu_Settings);
        bool* displaySubmenu = new bool;
        *displaySubmenu = false;
        //define submenu
        graph_Settings_Submenu_Settings.push_back(new Button("asd", nullptr, displaySubmenu, "\n"));
        //intigrate subemenu into menu
        this->settings.push_back(new Submenu(graph_Settings_Submenu_Menu, displaySubmenu, "\n"));
        //button for enabling the submenu
        auto updateSelection = [this, i, displaySubmenu](){
            if(this->selectedSubmenu == i+1){
                this->selectedSubmenu = 0;
            }else{
                this->selectedSubmenu = i+1;
            }
            std::cout << *displaySubmenu << std::endl;
        };
        this->settings.push_back(new Button(this->graphs[i]->getName(), updateSelection, displaySubmenu, "\n"));
    }

    auto addFunc = [this](){
        this->shouldUpdateFrame = true;
        this->addGraph();
    };
    this->settings.push_back(new Button("+New Graph", addFunc, nullptr, "\n"));
    this->frame = new MenuFrame(this->settings);
    this->shouldUpdateFrame = false;
}

void Menu_Graph::addGraph(){
    this->graphs.push_back(new Graph(GREEN, nullptr));
};

std::vector<Graph*>* Menu_Graph::getGraphs(){
    return &this->graphs;
}