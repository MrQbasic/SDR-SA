
#include <source/source.hpp>
#include <graph/graph.hpp>

bool display_menuAddSdr = true;

#include <gui/menus/addSdr.hpp>


void renderMenuBar(){
    //top Menu
    ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("Graphs")){
            for(auto g : *Graph::getGraphs()){
                if(ImGui::MenuItem(g->getName())){
                    g->toggleDisplayStatusMenu();
                }
            }

            if(ImGui::MenuItem("+ Graph")){
                Graph* g = new Graph(nullptr);
                Graph::addGraph(g);
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Sources")){
            //list all the sources in use
            for(auto src : *Source::getSources()){
                if(ImGui::BeginMenu(src->getName())){
                    
                    src->renderMenu();

                    ImGui::EndMenu();
                }
            }
            //opetions for adding
            if(ImGui::MenuItem("+ SDR")){
                display_menuAddSdr ^= true;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Chart")){
            

            ImGui::EndMenu();
        }

    ImGui::EndMainMenuBar();


    //windows submenues
    //--- sdr add menu
    if(display_menuAddSdr){
        renderAddSdrMenu();
    }
    //--- graph menus
    for(auto g : *Graph::getGraphs()){
        //automaticly desides to render it or not
        g->renderMenuSettings();
    }

}