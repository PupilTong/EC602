//copyright 2019 haoyangw haoyangw@bu.edu
#include <iostream>
#include <cstdint>
#include <ctime>
int main(){
    uint16_t  m=1;
    clock_t start_clock, end_clock;
    start_clock = clock();
    while (m>0) m++;
    end_clock = clock();
    double seconds = static_cast<double>(end_clock - start_clock) / CLOCKS_PER_SEC;



    std::cout << "estimated int8 time (nanoseconds): "
            << seconds /(double)(1<<8)* 1000000000 << std::endl;
    std::cout << "measured int16 time (microseconds): "
            << seconds*1000000.0 << std::endl;
    std::cout << "estimated int32 time (seconds): "
            << seconds * (double)(1<<16)  << std::endl;
            uint64_t converter = 0x1;
            converter = converter<<48;
    std::cout << "estimated int64 time (years): "
            << seconds * ((double)converter)/(3600.0*24.0*365.0) << std::endl;
}