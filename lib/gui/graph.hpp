#include <raylib.h>
#include <gui/def.hpp>
#include <sdr.hpp>
#include <scanner.hpp>

#pragma once

class Graph{
private:
    //general rendering
    static int posX;
    static int posY;
    static int width;
    static int height;
    static double dbTop, dbBottom;
    static long long freqStart, freqEnd;
    Color col;
    //scanner for spectrum
    SDR* sdr;
    double* specterum = nullptr;
    int spectrumSize = 0;
    bool scannerRunning = false;
    Scanner* scanner;

public:

    Graph(Color col, SDR* sdr){
        this->col = col;
        this->sdr = sdr;
    }

    static void setDimensions(int v_posX, int v_posY, int v_width, int v_height, double v_dbTop, double v_dbBottom, long long v_freqStart, long long v_freqEnd);

    static void renderGrapBackground();

    //expected to have as much data points as px in width
    void render();
};
