#include "Bar.h"

void Bar::incrementGlassCount() {
    while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::unique_lock<std::mutex> lock(mutex_);
    glassCount++;
    lock.unlock();
    cv_.notify_all();
    }
}

void Bar::refillBeer()  {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::unique_lock<std::mutex> lock(mutex_);
        if (beerCount <= 700) {
            beerCount = 10000;
            std::cout << "Wymieniono beczke. Pozostalo " << beerCount << "ml piwa." << std::endl;
        }
        lock.unlock();
        cv_.notify_all();
    }
}

void Bar::giveBeer() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (true) {
        cv_.wait(lock, [this] { return glassCount > 0 && beerCount > 700; });
        glassCount--;
        beerCount -= 700;
        std::cout << "Wydano piwo. Pozostale szklanki: " << glassCount << ", pozostalo : " << beerCount << "ml piwa" << std::endl;
    }
}