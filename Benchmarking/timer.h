//
// Created by Daniel Nykjaer on 28/05/2023.
//

#ifndef EKSAMEN2023_TIMER_H
#define EKSAMEN2023_TIMER_H
#include <chrono>

class timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> timePoint;
public:
    void start();
    double stop();
};
#endif //EKSAMEN2023_TIMER_H
