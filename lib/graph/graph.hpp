#pragma once

#include <source/source.hpp>
#include <vector>
#include <thread>


class Graph {
public:
    Graph();
    ~Graph();

    const char* getName();

    void renderMenuSettings();
    void renderGraph();

    void toggleDisplayStatusMenu();

    static void addGraph(Graph* graph);
    static bool removeGraph(Graph* graph);  //true if matching entry was found
    static std::vector<Graph*>* getGraphs();

private:
    void updater();

private:
    bool renderMenu = false;
    bool render     = true;

    int sourceIndex;
    Source* source;
    char* name;

    bool updaterRunning;
    bool updaterShouldRun;
    std::thread* updaterThread;

    static std::vector<Graph*> graphs;
};
