#include <sdr.hpp>
#include <thread>
#include <iostream>


class Scanner {
private:
    double** outputBufferVal;
    int**    outputBufferValCnt;
    int     outputBufferSize;
    int     graphType;
    long long start, end;

    SDR* sdr;

    bool run;
    bool died;
    std::thread* thread;

public:
    Scanner(SDR* sdr, double** bufferVal, int** bufferValCnt, int graphType, int size, long long start, long long end);

    ~Scanner();

    void execute();
    
    void stop();
};

