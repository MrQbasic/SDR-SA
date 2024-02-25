#include <sdr.hpp>
#include <thread>
#include <iostream>


class Scanner {
private:
    double* outputBuffer;
    int outputBufferSize;
    long long start, end;

    SDR* sdr;

    bool run;
    bool died;
    std::thread* thread;

public:
    Scanner(SDR* sdr, double* buffer, int size, long long start, long long end);

    void execute();
    
    void stop();
};

