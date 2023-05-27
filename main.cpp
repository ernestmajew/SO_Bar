#include "Bar.h"


int main() {
    Bar bar;    

    std::thread cleanGlasses(&Bar::incrementGlassCount, &bar);
    std::thread cleanGlassesSecond(&Bar::incrementGlassCount, &bar);
    std::thread refillBeer(&Bar::refillBeer, &bar);
    std::thread giveBeerThread(&Bar::giveBeer, &bar);
    std::thread printBarThread(&Bar::printBar, &bar);

    cleanGlasses.join();
    cleanGlassesSecond.join();
    refillBeer.join();
    giveBeerThread.join();
    printBarThread.join();

    return 0;
}
