#include <ncurses.h>
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
    }
}

void Bar::printBar() {
// g++ -std=c++20 -o main main.cpp bar.cpp -lncurses -lpthread
    initscr();
    refresh();

    WINDOW * beer = newwin(11, 10, 1, 0);
    box(beer, 0, 0);

    WINDOW * glasses = newwin(3, 10, 1, 15);
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
