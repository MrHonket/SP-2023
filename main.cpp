#include "Simulator/Helper.h"
#include "Simulator/reaction.h"
#include "Simulator/Simulator.h"
#include "Benchmarking/benchmarking.h"
#include <future>

//Functions for running the simulations for requirement 5
void performSimulationABC(const Reagent& A,const Reagent& B,const Reagent& C){
    auto lambda = 0.001;
    auto endTime = 1500;
    reaction ABC (LHS{{{"A",1}, {"C",1}}} >>= {{{"B",1}, {"C",1}}, lambda});

    auto s = reaction::state{};
    s.update(A.name, A.volume);
    s.update(B.name, B.volume);
    s.update(C.name, C.volume);

    //construct the simulator
    Simulator ABCSim {{ABC}, s};
    ABCSim.runSimulation(endTime);
}

void Requirement51(){
    std::cout << "-----Doing simulations for requirement 5.1-----" << std::endl;
    performSimulationABC(Reagent{"A",100},Reagent{"B",0},Reagent{"C",1});
    std::cout << std::endl;
    performSimulationABC(Reagent{"A",100},Reagent{"B",0},Reagent{"C",2});
    std::cout << std::endl;
    performSimulationABC(Reagent{"A",50},Reagent{"B",50},Reagent{"C",1});
    std::cout << "-----Finished simulations for requirement 5.1-----" << std::endl;
}

void performSimulationCR(double simtime){
    auto CRStates = reaction::state{};
    auto alphaA = 50.0;
    auto alpha_A = 500.0;
    auto alphaR = 0.01;
    auto alpha_R = 50.0;
    auto betaA = 50.0;
    auto betaR = 5.0;
    auto gammaA = 1.0;
    auto gammaR = 1.0;
    auto gammaC = 2.0;
    auto deltaA = 1.0;
    auto deltaR = 0.2;
    auto deltaMA = 10.0;
    auto deltaMR = 0.5;
    auto thetaA = 50.0;
    auto thetaR = 100.0;

    auto DA = Reagent{"DA", 1};
    auto D_A = Reagent{"D_A", 1};
    auto DR = Reagent{"DR", 1};
    auto D_R = Reagent{"D_R", 1};
    auto MA = Reagent{"MA", 1};
    auto MR = Reagent{"MR", 1};
    auto A = Reagent{"A", 1};
    auto R = Reagent{"R", 1};
    auto C = Reagent{"C", 1};

    //Initialize the start values for all the reagents
    CRStates.update(DA.name, 1);
    CRStates.update(D_A.name, 0);
    CRStates.update(DR.name, 1);
    CRStates.update(D_R.name, 0);
    CRStates.update(MA.name, 0);
    CRStates.update(MR.name, 0);
    CRStates.update(A.name, 0);
    CRStates.update(R.name, 0);
    CRStates.update(C.name, 0);

    //define a list of reactions
    const std::initializer_list<reaction> initializerList = {
            reaction(LHS{{A, DA}} >>= {{D_A}, gammaA}),
            reaction(LHS{D_A} >>= {{DA,A}, thetaA}),
            reaction(LHS{A,DR} >>= {{D_R}, gammaR}),
            reaction(LHS{D_R} >>= {{DR, A}, thetaR}),
            reaction(LHS{D_A} >>= {{MA, D_A}, alpha_A}),
            reaction(LHS{DA} >>= {{MA, DA}, alphaA}),
            reaction(LHS{D_R} >>= {{MR, D_R}, alpha_R}),
            reaction(LHS{DR} >>= {{MR, DR}, alphaR}),
            reaction(LHS{MA} >>= {{MA, A}, betaA}),
            reaction(LHS{MR} >>= {{MR,R}, betaR}),
            reaction(LHS{A,R} >>= {{C}, gammaC}),
            reaction(LHS{C} >>= {{R}, deltaA}),
            reaction(LHS{A} >>= {{}, deltaA}),
            reaction(LHS{R} >>= {{}, deltaR}),
            reaction(LHS{MA} >>= {{}, deltaMA}),
            reaction(LHS{MR} >>= {{}, deltaMR})
    };

    Simulator CRSimulator {initializerList, CRStates};
    CRSimulator.runSimulation(simtime);
}

std::vector<reaction::state> performSimulationCovid(double simtime, double Nstart){
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

// Functions for calculating the Local max and average max for all performed simulations for requirement 7 / 10
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

// Functions for calculating the Local mean and  mean for all performed simulations for requirement 8 / 10
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


//Benchmark for multicore performance using futures
//Computed average hospitalized H for N_NJ = 73.8249 and N_DK =
//Requirement 10
void runBenchmarks(){
    benchmarking::runBenchmark(1,[=]()->void{
        constexpr int simulatorCount = 2;
        const double simTime = 100;
        const double N = 10000;
        std::vector<std::future<std::vector<reaction::state>>> futures;

        std::cout << "Starting multi-core simulation..." << std::endl;

        for(int i = 0; i < simulatorCount; ++i){
            std::cout << "launching nr: " << i << std::endl;
            auto future = std::async(std::launch::async, performSimulationCovid, simTime, N);
            futures.push_back(std::move(future));
        }

        //Requirement 7
        //futureMax("H",std::move(futures));

        //Requirement 8
        futureMean("H",std::move(futures));
    });

    //Benchmark for single-core performance
    //Requirement 10
    std::cout << "----- New Benchmarks -----" << std::endl;
    benchmarking::runBenchmark(1,[=]()->void{
        constexpr int simulatorCount = 1;
        const double simTime = 100;
        const double N = 10000;
        double SCMin = 0.0;

        std::cout << "Starting single-core benchmark simulation..." << std::endl;
        for(int i = 0; i < simulatorCount; i++){
            auto result = performSimulationCovid(simTime, N);
            SCMin += meanCalculator("H",result);
        }
        std::cout << "mean H: " << SCMin/simulatorCount << std::endl;
    });
}

int main(){
    //Requirement 5.1
    //Requirement51();

    //Requirement 5.2
    //performSimulationCR(100);

    //Requirement 5.3
    //auto a = performSimulationCovid(100,10000);

    //Requirement 10
    runBenchmarks();

    return 0;
}
