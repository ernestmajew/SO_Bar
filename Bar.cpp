#include <ncurses.h>
#include <random>
#include "Bar.h"

void Bar::incrementGlassCount() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> sleepDist(2, 5);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(sleepDist(gen)));

        std::unique_lock<std::mutex> lock(glassMutex_);
        glassCount++;
        lock.unlock();

        glassCV_.notify_all();
    }
}

void Bar::refillBeer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> sleepDist(7, 10);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(sleepDist(gen)));

        std::unique_lock<std::mutex> lock(beerMutex_);
        if (beerCount <= 700) {
            beerCount = 10000;
        }
        lock.unlock();

        beerCV_.notify_all();
    }
}

void Bar::giveBeer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> sleepDist(1, 3);

    std::unique_lock<std::mutex> glassLock(glassMutex_, std::defer_lock);
    std::unique_lock<std::mutex> beerLock(beerMutex_, std::defer_lock);
    std::lock(glassLock, beerLock);

    while (true) {
        glassCV_.wait(glassLock, [this] { return glassCount > 0 && beerCount > 700; });

        glassCount--;
        beerCount -= 700;

        std::this_thread::sleep_for(std::chrono::seconds(sleepDist(gen)));
    }
}

void Bar::printBar() {
    // g++ -std=c++20 -o main main.cpp bar.cpp -lncurses -lpthread
    initscr();
    refresh();

    WINDOW* beer = newwin(11, 10, 1, 0);
    box(beer, 0, 0);

    WINDOW* glasses = newwin(3, 10, 1, 15);
    box(glasses, 0, 0);

    printw("   Beer\t\t Glasses");
    refresh();

    while (true) {
        wrefresh(beer);
        wrefresh(glasses);

        for (int i = 1; i < 10; i++) {
            for (int j = 1; j < 9; j++) {
                mvwaddch(beer, i, j, '#');
            }
        }

        for (int i = 1; i < 10 - (beerCount / 1000); i++) {
            for (int j = 1; j < 9; j++) {
                mvwaddch(beer, i, j, ' ');
            }
        }

        for (int i = 1; i < 9; i++) {
            mvwaddch(glasses, 1, i, ' ');
        }

        for (int i = 1; i <= glassCount; i++) {
            mvwaddch(glasses, 1, i, '#');
        }

        wrefresh(beer);
        wrefresh(glasses);
    }

    endwin();
}