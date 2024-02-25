#include <gui/graph.hpp>
#include <tools/freq.hpp>
#include <iostream>

int Graph::posX = 0;
int Graph::posY = 0;
int Graph::width = 0;
int Graph::height = 0;
long long Graph::freqStart = 0;
long long Graph::freqEnd = 0;
double Graph::dbBottom = 0;
double Graph::dbTop = 0;


void Graph::setDimensions(int v_posX, int v_posY, int v_width, int v_height, double v_dbTop, double v_dbBottom, long long v_freqStart, long long v_freqEnd){
    posX = v_posX;
    posY = v_posY;
    width = v_width;
    height = v_height;
    dbBottom = v_dbBottom;
    dbTop = v_dbTop;
    freqEnd = v_freqEnd;
    freqStart = v_freqStart;
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
    if(this->sdr->state == 0){
        if(this->scannerRunning){
            //stop scanner
        }
        return;
    }
    if(this->spectrumSize != this->width){
        if(this->specterum != nullptr){
            delete this->specterum;
        }
        this->specterum = new double[width] {10.0,100.0};
        for(int i=0; i<width; i++){
            this->specterum[i] = 0.0;
        }
        this->scanner = new Scanner(this->sdr, this->specterum, width, freqStart, freqEnd);
        this->spectrumSize = width;
    }
    //render spectrum
    double pxPerDb = (double) height / (dbTop-dbBottom);
    double zeroPosY = ((double) posY + pxPerDb*dbTop);
    for(int x=1; x<width; x++){
        DrawLine(
            posX+x-1,
            zeroPosY-pxPerDb*clampDouble(this->specterum[x-1], dbTop, dbBottom), 
            posX+x, 
            zeroPosY-pxPerDb*clampDouble(this->specterum[x], dbTop, dbBottom), 
            this->col
        );
    }
}