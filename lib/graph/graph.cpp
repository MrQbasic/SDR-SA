#include <graph/graph.hpp>
#include <ImGui/imgui.h>

#include <iostream>

//static methods for keeping all graphs together

std::vector<Graph*> Graph::graphs;
void Graph::addGraph(Graph* graph){
    graphs.push_back(graph);
}
bool Graph::removeGraph(Graph* graph){
    int i=0;
    for(Graph* g : graphs){
        if(g == graph){
            graphs.erase(graphs.begin() + i);
            return true;
        }
        i++;
    }
    return false;
}
std::vector<Graph*>* Graph::getGraphs(){
    return &graphs;
}



//constructor / Destructor

Graph::Graph(Source* source){
    this->name = new char[30] {"new Graph"};   //TODO? hardcoded char limit is fine
}
Graph::~Graph(){
    delete this->name;
}


// get and set (not buffered to preserver render time)
const char* Graph::getName(){
    return (const char*) name;
}
void Graph::toggleDisplayStatusMenu(){
    this->renderMenu ^= 1;
}



//render functions

void Graph::renderGraph(){
    return;
}
void Graph::renderMenuSettings(){
    if(!renderMenu) return;

    ImGui::Begin("Graph Setting");
        ImGui::ListBox("Source", &this->sourceIndex,  );
        //Names
        ImGui::InputText("Name", name, 30, ImGuiInputTextFlags_None );
        //remove button
        if(ImGui::Button("Remove")){
            Graph::removeGraph(this);
            delete this;
            ImGui::End();
            return;
        }
    ImGui::End();
}