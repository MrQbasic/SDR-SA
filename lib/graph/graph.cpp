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
    updaterRunning = false;
    updaterShouldRun = false;
}

Graph::Graph(Source* src){
    //auto gen the name
    char* srcName = (char*) src->getName();
    int nameLen = std::snprintf(nullptr, 0, "Graph: %s", srcName);
    this->name = new char[nameLen];
    std::snprintf(this->name, nameLen, "Graph: %s", srcName);
    //do the rest of the setup
    this->source = src;
    renderMenu = false;
    render = true;
    updaterRunning = false;
    updaterShouldRun = true;
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


void Graph::updater(){
    this->updaterRunning = true;
    while(this->updaterShouldRun){
        try{
            this->source->updateData(100000000);
        }catch (const std::exception_ptr& e){
            this->updaterShouldRun = false;
            std::cout << "asd" << std::endl;
        }
    }
    this->updaterRunning = false;
    std::cout << "Updater died" << std::endl;
}


//render functions
void Graph::renderGraph(){
    if(!render) return;
        if(updaterRunning == false && updaterShouldRun == true){
            std::cout << "Starting updater" << std::endl;
            this->updaterThread = new std::thread(&Graph::updater, this);
        }
        //render Graph
        if(this->source == 0) return;
        double* dataX = nullptr;
        double* dataY = nullptr;
        int cnt = this->source->getData(&dataX, &dataY);
        if(dataX == nullptr || dataY == nullptr) return;
        ImPlot::PlotLine(this->name, dataX, dataY, cnt-1);
    return;
}

void Graph::renderMenuSettings(){
    if(!renderMenu) return;

    ImGui::Begin("Graph Setting", &(this->renderMenu), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


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
                if( src == (Source*)this) continue;
                newSourceIndex--;
                if(newSourceIndex == -1){
                    this->source = (*sources)[i];
                    this->render = true;
                    this->updaterShouldRun = true;
                }
            }
        }


        //Name of Graph
        ImGui::InputText("Name", name, 30, ImGuiInputTextFlags_None );

        //Hide graph
        ImGui::Checkbox("Render on Chart", & this->render);

        //remove button
        if(ImGui::Button("Remove")){
            this->updaterShouldRun = false;
            this->updaterThread->join();
            Graph::removeGraph(this);
            delete this;
            ImGui::End();
            return;
        }
        
    ImGui::End();
}