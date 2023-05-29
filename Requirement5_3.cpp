//
// Created by Daniel Nykjaer on 27/05/2023.
//
#include "Simulator/Helper.h"
#include "Simulator/reaction.h"
#include "Simulator/Simulator.h"
#include "Benchmarking/benchmarking.h"
#include <future>

double meanCalculator(const std::string &key, const std::vector<reaction::state> history){
    double mean = 0.0;
    for(auto i : history){
        mean += i.lookup(key).value();
    }
    return mean / history.size();

}

void futureMean(const std::string &key, std::vector<std::future<std::vector<reaction::state>>>&& futures){
    double mean;
    std::cout << "calculating Mean" << std::endl;

    for(auto& i : futures){
        auto result = i.get();
        mean += meanCalculator(key, result);
    }

    std::cout << "Mean " << key << ": " << mean / futures.size() << std::endl;
}

double localMax(const std::string &key, const std::vector<reaction::state> history){
    double localMax = 0.0;
    for(auto i : history){
        if(i.lookup(key).value() > localMax)
            localMax = i.lookup(key).value();
    }
    return localMax;

}

void futureMax(const std::string &key, std::vector<std::future<std::vector<reaction::state>>>&& futures){
    double Max = 0.0;
    std::cout << "calculating Max" << std::endl;

    for(auto& i : futures){
        auto result = i.get();
        if(localMax(key,result) > Max)
            Max = localMax(key,result);
    }

    std::cout << "Max " << key << ": " << Max << std::endl;
}

std::vector<reaction::state> performSimulation(double simtime, double Nstart){
    auto CovidStates = reaction::state {};
    const double eps = 0.0009; // initial fraction of infectious
    const auto I0 = double(std::round(eps*Nstart)); // initial infectious
    const auto E0 = double(std::round(eps*Nstart*15)); // initial exposed
    const double S0 = Nstart-I0-E0; // initial susceptible
    const double R0 = 2.4; // basic reproductive number (initial, without lockdown etc)
    const double alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
    const double gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
    const double beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
    const double P_H = 0.9e-3; // probability of hospitalization
    const double kappa = gamma * P_H*(1.0-P_H); // hospitalization rate (I -> H)
    const double tau = 1.0/10.12; // recovery/death rate in hospital (H -> R) ~10.12 days

    auto S = Reagent{"S", 1};
    auto E = Reagent{"E", 1};
    auto I = Reagent{"I", 1};
    auto H = Reagent{"H", 1};
    auto R = Reagent{"R", 1};

    CovidStates.store(S.name, S0);
    CovidStates.store(E.name, E0);
    CovidStates.store(I.name, I0);
    CovidStates.store(H.name, 0);
    CovidStates.store(R.name, 0);

    const std::initializer_list<reaction> reactions = {
            reaction(LHS {{S,I}} >>= {{E,I}, beta/Nstart}), // susceptible becomes exposed through infectious
            reaction(LHS  {{E}} >>= {{I}, alpha}),// exposed becomes infectious
            reaction(LHS  {{I}}>>= {{{R.name, R.volume}}, gamma}), // infectious becomes removed
            reaction(LHS {{I}} >>= {{H}, kappa}), // infectious becomes hospitalized
            reaction(LHS {{H.name, H.volume}} >>= {{R}, tau})    // hospitalized becomes removed
    };

    Simulator CovidSimulator {reactions, CovidStates};
    return CovidSimulator.runSimulation(100);
}

void Requirement8and10(){
    //Benchmark for multicore performance using futures
    //Computed average hospitalized H for N_NJ = 73.8249 and N_DK =
    //Requirement 8 and 10
    benchmarking::runBenchmark(1,[=]()->void{
        constexpr int simulatorCount = 2;
        const double simTime = 100;
        const double N = 10000;
        std::vector<std::future<std::vector<reaction::state>>> futures;

        std::cout << "Starting multi-core simulation..." << std::endl;

        for(int i = 0; i < simulatorCount; ++i){
            std::cout << "launching nr: " << i << std::endl;
            auto future = std::async(std::launch::async, performSimulation, simTime, N);
            futures.push_back(std::move(future));
        }

        //Requirement 7
        //futureMax("H",std::move(futures));

        //Requirement 8
        futureMean("H",std::move(futures));
    });

    //Benchmark for single-core performance
    //Requirement 10
    std::cout << "----- New Sim -----" << std::endl;
    benchmarking::runBenchmark(1,[=]()->void{
        constexpr int simulatorCount = 20;
        const double simTime = 100;
        const double N = 10000;
        double SCMin = 0.0;

        std::cout << "Starting single-core benchmark simulation..." << std::endl;
        for(int i = 0; i < simulatorCount; i++){
            auto result = performSimulation(simTime, N);
            SCMin += meanCalculator("H",result);
        }
        std::cout << "mean H: " << SCMin/simulatorCount << std::endl;
    });
}

int main(){
    Requirement8and10();
    return 0;
}
