#include <raylib.h>

#pragma once

const char* freqString[] ={"Hz","kHz","MHz","GHz"};

const char* freqToString(long long freq, int decimals){

    int stringIndex = 0;
    long long divider = 1000;

    while(freq/divider > 0){
        stringIndex++;
        divider *= 1000;
    }

    double freqShortend = (double)freq/(double)(divider/1000);
    
    return(TextFormat("%.*f%s",decimals,freqShortend,freqString[stringIndex]));
}