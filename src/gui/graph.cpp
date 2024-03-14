#include <gui/graph.hpp>
#include <tools/freq.hpp>
#include <iostream>
#include <math.h>

int Graph::posX = 0;
int Graph::posY = 0;
int Graph::width = 0;
int Graph::height = 0;
long long Graph::freqStart = 0;
long long Graph::freqEnd = 0;
double Graph::dbBottom = 0;
double Graph::dbTop = 0;
int Graph::globalConfigID = 0;


void Graph::setDimensions(int v_posX, int v_posY, int v_width, int v_height, double v_dbTop, double v_dbBottom, long long v_freqStart, long long v_freqEnd){
    //only check on those relevant for the scanner not only for rendering
    bool detectedChange = false;
    //dimensions
    detectedChange |= posX != v_posX;
    posX = v_posX;
    detectedChange |= posY != v_posY;
    posY = v_posY;
    detectedChange |= width != v_width;
    width = v_width;
    height = v_height;
    //db
    dbBottom = v_dbBottom;
    dbTop = v_dbTop;
    //freqs
    detectedChange |= freqEnd != v_freqEnd;
    freqEnd = v_freqEnd;
    detectedChange |= freqStart != v_freqStart;
    freqStart = v_freqStart;
    //if there is a change inc conf ig
    if(detectedChange){
        globalConfigID++;
    }
}

void Graph::renderGrapBackground(){
    //Draw bg and outline
    DrawRectangle(posX, posY, width, height, COLOR_BACKGROUND);
    DrawRectangleLines(posX, posY, width, height, COLOR_IDLE);
    //Draw horiz. lines + db values
    double pxPerDivX = (double)(width) / (double)(GRAPH_X_DIVS+1);
    double textShiftPerDivX = (double) -MeasureText(freqToString(freqEnd, GRAPH_FREQ_DECMIALS),GRAPH_TEXT_SIZE) / (double) (GRAPH_X_DIVS+1);
    double hzPerDiv  =  (double)(freqEnd-freqStart) / (double) (GRAPH_X_DIVS+1);
    for(int x=0; x<=GRAPH_X_DIVS+1; x++){
        int divPosX = (int) (pxPerDivX*x) + posX;
        int textShift = (int) (textShiftPerDivX*x);
        const char* text = freqToString(freqStart + (long long)(hzPerDiv*x), GRAPH_FREQ_DECMIALS);
        DrawText(text, divPosX+textShift, posY-15, GRAPH_TEXT_SIZE, GRAPH_TEXT_COLOR);
        DrawText(text, divPosX+textShift, posY+height+5, GRAPH_TEXT_SIZE, GRAPH_TEXT_COLOR);
        DrawLine(divPosX, posY, divPosX, posY+height, COLOR_IDLE);
    }
    double pxPerDivY = (double)height / (double)(GRAPH_Y_DIVS+1);
    double dbPerDiv  = (dbBottom-dbTop) / (GRAPH_Y_DIVS+1);
    for(int y=0; y<=GRAPH_Y_DIVS+1; y++){
        int divPosY = (int) (pxPerDivY*y) + posY;
        const char* text = TextFormat("%2.fdB", dbTop + (dbPerDiv*y));
        int textWidth = MeasureText(text, GRAPH_TEXT_SIZE);
        DrawText(text, posX-textWidth-5, divPosY-(GRAPH_TEXT_SIZE/2), GRAPH_TEXT_SIZE, GRAPH_TEXT_COLOR);
        DrawText(text, posX+width+5,     divPosY-(GRAPH_TEXT_SIZE/2), GRAPH_TEXT_SIZE, GRAPH_TEXT_COLOR);
        DrawLine(posX, (int) (pxPerDivY*y) + posY, posX+width, (int) (pxPerDivY*y) + posY, COLOR_IDLE);
    }
}


double clampDouble(double n, double max, double min){
    if(n < min) return min;
    if(n > max) return max;
    return n;
}


void Graph::render(){
    //check if we need to render the graph
    if(this->sdr->state != 1){
        //check if we need to kill old scanner and disconnect unused sdr
        if(this->sdr->state == 2){
            delete this->scanner;
            this->scanner = nullptr;
            this->sdr->disconnect();
            this->spectrumSize = 0;
        }
        return;
    }
    //check if the cfg changed
    if(globalConfigID != this->configID){
        this->configID = globalConfigID;
        //check if old scanner has to be killed
        if(this-> scanner != nullptr){
            delete this->scanner;
            this->scanner == nullptr;
        }
        //create new scanner
        this->scanner = new Scanner(
            this->sdr, &this->spectrumVal, &this->spectrumValCnt, this->type,
            width, freqStart, freqEnd);
        this->spectrumSize = width;
    }
    //render spectrum
    double pxPerDb = (double) height / (dbTop-dbBottom);
    double zeroPosY = ((double) posY + pxPerDb*dbTop);
    for(int x=1; x<width; x++){
        DrawLine(
            posX+x-1,
            zeroPosY-pxPerDb*clampDouble(this->spectrumVal[x-1]/abs(spectrumValCnt[x-1]), dbTop, dbBottom), 
            posX+x, 
            zeroPosY-pxPerDb*clampDouble(this->spectrumVal[x]/abs(spectrumValCnt[x]), dbTop, dbBottom), 
            this->col
        );
    }
}