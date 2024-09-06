#include <gui/menu.hpp>
#include <gui/graph.hpp>
#include <vector>

class Menu_Graph : public Menu {
public:
    Menu_Graph();
    std::vector<Graph*>* getGraphs();
    void update() override;

private:
    void addGraph();

public:
    /*
    So you need to make the graphs actually be the fft lines
    they need an on off setting
    also custom scripting (lua maybe ?) to merge the graphs / 
    Tools menu pushs data from existing graphs to others (auto creation would be nice tho...
    )

    You can do the computation in the output loops as maybe some graphs are hidden layers

    Also maybe graph renderer class#

    No no tool menu as submenu of graph settings


    GRAPHS NEED TO WORK WITHOUT AN SDR AND SDRS need to push into the graph buffer

    ALSO WE NEED ANALYSE TOOLS
        -LO calculations
        -Harmonic shit
        -S21 maybe ?

    BLADE SUPPORT
    */

    std::vector<Graph*> graphs;

    i_MenuFrame* frame;
    std::vector<Setting*> settings;

    bool shouldUpdateFrame = false;

    int selectedSubmenu = 0;
};