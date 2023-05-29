//
// Created by Daniel Nykjaer on 28/05/2023.
//

#ifndef EKSAMEN2023_BENCHMARKING_H
#define EKSAMEN2023_BENCHMARKING_H

#include "timer.h"
#include <functional>
#include <iostream>

class benchmarking {
public:
    static void runBenchmark( int runs, const std::function<void (void)> &func);
};


#endif //EKSAMEN2023_BENCHMARKING_H
