//
// Created by Daniel Nykjaer on 28/05/2023.
//

#include "timer.h"

void timer::start() {
    timePoint = std::chrono::high_resolution_clock::now();
}

double timer::stop() {
    auto endTimePoint = std::chrono::high_resolution_clock::now();
    auto timePassed = std::chrono::duration<double, std::milli>(endTimePoint - timePoint).count();
    return timePassed;
}
