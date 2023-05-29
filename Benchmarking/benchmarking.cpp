//
// Created by Daniel Nykjaer on 28/05/2023.
//

#include "benchmarking.h"

void benchmarking::runBenchmark(int runs, const std::function<void()> &func){
    std::cout << "Starting benchmarks" << std::endl;

    double total = 0.0;
    for (int i = 0; i < runs; ++i){
        timer clock;
        clock.start();

        func();

        total += clock.stop();
    }

    double totalruntime = total / runs;
    std::cout << "The total runtime was: " << totalruntime << std::endl;
}
