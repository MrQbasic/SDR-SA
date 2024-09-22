
#include <source/source.hpp>
#include <graph/graph.hpp>

bool display_menuAddSdr = true;

#include <gui/menus/addSdr.hpp>
#include <gui/menus/addModifier.hpp>


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
                Graph* g = new Graph();
                Graph::addGraph(g);
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Sources")){
            //list all the sources in use
            bool displayHeader = true;
            for(auto src : *Source::getSources()){
                //render header once
                if(displayHeader){
                    ImGui::BeginDisabled();
                    ImGui::MenuItem("--Active sources--");
                    ImGui::EndDisabled();
                    displayHeader = false;
                }
                //list all sdrs
                if(ImGui::BeginMenu(src->getName())){
                    src->renderMenu();
                    ImGui::EndMenu();
                }
            }
            //opetions for adding
            ImGui::BeginDisabled();
            ImGui::MenuItem("--Add--");
            ImGui::EndDisabled();
            //-SDR
            if(ImGui::MenuItem("+ SDR")){
                display_menuAddSdr ^= true;
            }
            //-Modifier
            
            if(ImGui::BeginMenu("+ Modifier")){
                renderAddModifier();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Chart")){
            

            ImGui::EndMenu();
        }

        char* fpsDisplay = new char[20];
        snprintf(fpsDisplay, 20, "FPS: %f", ImGui::GetIO().Framerate);
        ImGui::BeginMenu(fpsDisplay);

    ImGui::EndMainMenuBar();


    //windows submenues
    //--- sdr add menu
    if(display_menuAddSdr){
        renderAddSdrMenu(&display_menuAddSdr);
    }
    //--- graph menus
    for(auto g : *Graph::getGraphs()){
        //automaticly desides to render it or not
        g->renderMenuSettings();
    }

}