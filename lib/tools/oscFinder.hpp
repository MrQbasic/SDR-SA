#include <ImGui/imgui.h>
#include <graph/graph.hpp>
 

Graph* currentGraph = nullptr;

void renderTool_OscFinder(){
    if(ImGui::Begin("Oscillator Finder")){
        
        //Selecting the Graph to pull the data from
        std::vector<Graph*> graphs = *(Graph::getGraphs());
        int graphCount = graphs.size();
        char** names = new char*[graphCount];
        int currentGraphIndex = -1;
        for(int i=0; i<graphCount; i++){
            names[i] = (char*) graphs[i]->getName();
            if(graphs[i] == currentGraph) currentGraphIndex = i; 
        }
        int newGraphIndex = currentGraphIndex;
        ImGui::ListBox("Source", &newGraphIndex, names, graphCount, -1);
        delete names;
        if(currentGraphIndex != newGraphIndex){
            currentGraph = graphs[newGraphIndex];
        }

        

        

        ImGui::End(); 
    }
}