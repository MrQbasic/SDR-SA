#include <graph/graph.hpp>
#include <source/source.hpp>

#include <ImGui/imgui.h>
#include <ImGui/implot.h>

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

Graph::Graph(){
    this->name = new char[30] {"new Graph"};   //TODO? hardcoded char limit is fine
    renderMenu = true;
    render = false;
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
    if(!render) return;
        if(this->source == 0) return;
        double* dataX;
        double* dataY;
        int cnt = this->source->getData(&dataX, &dataY);
        ImPlot::PlotLine(this->name, dataY, dataX, cnt);

    return;
}
void Graph::renderMenuSettings(){
    if(!renderMenu) return;

    ImGui::Begin("Graph Setting");

        std::vector<Source*>* sources = Source::getSources();
        this->sourceIndex = -1;
        const char** names = (const char**) new char* [sources->size()] ;
        int cnt = 0;
        for(auto src: *sources){
            names[cnt] = src->getName();
            cnt++;
        }
        ImGui::ListBox("Source", &this->sourceIndex, names, sources->size(), -1);
        if(sourceIndex != -1){
            this->source = (*sources)[this->sourceIndex];
            this->render = true;
        }

        //Name of Graph
        ImGui::InputText("Name", name, 30, ImGuiInputTextFlags_None );

        //Hide graph
        ImGui::Checkbox("Render on Chart", & this->render);

        //remove button
        if(ImGui::Button("Remove")){
            Graph::removeGraph(this);
            delete this;
            ImGui::End();
            return;
        }

        ImGui::SameLine();
        //Close button
        if(ImGui::Button("Close")){
            this->renderMenu = false;
        }
    ImGui::End();
}