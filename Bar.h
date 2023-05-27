#ifndef SO_BAR_BAR_H
#define SO_BAR_BAR_H
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Bar {
public:
    int glassCount;
    int beerCount;

    Bar() : glassCount(0), beerCount(10000) {}

    void giveBeer();
    void incrementGlassCount();
    void refillBeer();
    void printBar();

private:
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif //SO_BAR_BAR_H
