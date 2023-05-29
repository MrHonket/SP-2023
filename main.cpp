#include "Simulator/Simulator.h"
#include "Benchmarking/benchmarking.h"
#include <future>

//Functions for running the simulations for requirement 5
void performSimulationABC(const Reagent& A,const Reagent& B,const Reagent& C){
    //Initialize the start values for all reagents provided by the function input
    auto ABCStates = reaction::state{};
    ABCStates.update(A.name, A.volume);
    ABCStates.update(B.name, B.volume);
    ABCStates.update(C.name, C.volume);

    //Initialize the consumption rates
    auto lambda = 0.001;

    //Initialize the Reagents needed for the CR Reactions
    auto ABC_A = Reagent{"A", 1};
    auto ABC_B = Reagent{"B", 1};
    auto ABC_C = Reagent{"C", 1};

    //Initialize the Reagents needed for the ABC Reactions
    const std::initializer_list<reaction> ABCReactions = {
            reaction(LHS{{ABC_A, ABC_C}} >>= {{ABC_B,ABC_C}, lambda}),
    };

    //construct the simulator
    Simulator ABCSim {{ABCReactions}, ABCStates};

    //Initalize the runTime and run the simulator
    auto runTime = 1500;
    ABCSim.runSimulation(runTime);
}

void performSimulationABC(){
    std::cout << "-----Doing simulations for requirement 5.1-----" << std::endl;
    performSimulationABC(Reagent{"A",100},Reagent{"B",0},Reagent{"C",1});
    std::cout << std::endl;
    performSimulationABC(Reagent{"A",100},Reagent{"B",0},Reagent{"C",2});
    std::cout << std::endl;
    performSimulationABC(Reagent{"A",50},Reagent{"B",50},Reagent{"C",1});
    std::cout << "-----Finished simulations for requirement 5.1-----" << std::endl;
}

void performSimulationCR(double simtime){
    //Initialize the start values for all the reagents
    auto CRStates = reaction::state{};
    CRStates.update("DA", 1);
    CRStates.update("D_A", 0);
    CRStates.update("DR", 1);
    CRStates.update("D_R", 0);
    CRStates.update("MA", 0);
    CRStates.update("MR", 0);
    CRStates.update("A", 0);
    CRStates.update("R", 0);
    CRStates.update("C", 0);

    //Initialize the consumption rates
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

    //Initialize the Reagents needed for the CR Reactions
    auto DA = Reagent{"DA", 1};
    auto D_A = Reagent{"D_A", 1};
    auto DR = Reagent{"DR", 1};
    auto D_R = Reagent{"D_R", 1};
    auto MA = Reagent{"MA", 1};
    auto MR = Reagent{"MR", 1};
    auto A = Reagent{"A", 1};
    auto R = Reagent{"R", 1};
    auto C = Reagent{"C", 1};

    //Define CR Reactions
    const std::initializer_list<reaction> CRReactions = {
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

    //Construct the CR Simulator
    Simulator CRSimulator {CRReactions, CRStates};

    //Run the CR simulation
    CRSimulator.runSimulation(simtime);
}

std::vector<reaction::state> performSimulationCovid(double simtime, double Nstart){
    //Initialize the consumption rates first as these are needed for the initial states
    const double eps = 0.0009;
    const auto I0 = double(std::round(eps*Nstart));
    const auto E0 = double(std::round(eps*Nstart*15));
    const double S0 = Nstart-I0-E0;
    const double R0 = 2.4;
    const double alpha = 1.0 / 5.1;
    const double gamma = 1.0 / 3.1;
    const double beta = R0 * gamma;
    const double P_H = 0.9e-3;
    const double kappa = gamma * P_H*(1.0-P_H);
    const double tau = 1.0/10.12;

    //Initialize the start values for all the reagents
    auto CovidStates = reaction::state {};
    CovidStates.store("S", S0);
    CovidStates.store("E", E0);
    CovidStates.store("I", I0);
    CovidStates.store("H", 0);
    CovidStates.store("R", 0);

    //Initialize the Reagents needed for the Covid Reactions
    auto S = Reagent{"S", 1};
    auto E = Reagent{"E", 1};
    auto I = Reagent{"I", 1};
    auto H = Reagent{"H", 1};
    auto R = Reagent{"R", 1};

    //Define Covid Reactions
    const std::initializer_list<reaction> reactions = {
            reaction(LHS {{S,I}} >>= {{E,I}, beta/Nstart}), // susceptible becomes exposed through infectious
            reaction(LHS  {{E}} >>= {{I}, alpha}),// exposed becomes infectious
            reaction(LHS  {{I}}>>= {{{R.name, R.volume}}, gamma}), // infectious becomes removed
            reaction(LHS {{I}} >>= {{H}, kappa}), // infectious becomes hospitalized
            reaction(LHS {{H.name, H.volume}} >>= {{R}, tau})    // hospitalized becomes removed
    };

    //Construct the Covid Simulator
    Simulator CovidSimulator {reactions, CovidStates};

    //Run the Covid simulation
    return CovidSimulator.runSimulation(100);
}

//Requirement 7
//Estimated peak for hospitalized H for N_NJ = 123 and N_DK = 1187
template<typename T>
void monitorState(STable<double> currentState) {
    static double maxReagentValue = 0.0;  // Initialize with the minimum possible value
    const std::string key = "H";

    // Find the maximum value of a specific reagent
    if (currentState.contains(key)) {
        const auto& reagentAValue = currentState.lookup(key);
        if (reagentAValue.has_value() && reagentAValue.value() > maxReagentValue) {
            maxReagentValue = reagentAValue.value();
        }
    }

    // Print the maximum value
    std::cout << "Max value of H: " << maxReagentValue << std::endl;

}

void performSimulationRequirementSeven(){
    //Initialize Nstart
    const auto Nstart = 10000;

    //Initialize the consumption rates first as these are needed for the initial states
    const double eps = 0.0009;
    const auto I0 = double(std::round(eps*Nstart));
    const auto E0 = double(std::round(eps*Nstart*15));
    const double S0 = Nstart-I0-E0;
    const double R0 = 2.4;
    const double alpha = 1.0 / 5.1;
    const double gamma = 1.0 / 3.1;
    const double beta = R0 * gamma;
    const double P_H = 0.9e-3;
    const double kappa = gamma * P_H*(1.0-P_H);
    const double tau = 1.0/10.12;

    //Initialize the start values for all the reagents
    auto CovidStates = reaction::state {};
    CovidStates.store("S", S0);
    CovidStates.store("E", E0);
    CovidStates.store("I", I0);
    CovidStates.store("H", 0);
    CovidStates.store("R", 0);

    //Initialize the Reagents needed for the Covid Reactions
    auto S = Reagent{"S", 1};
    auto E = Reagent{"E", 1};
    auto I = Reagent{"I", 1};
    auto H = Reagent{"H", 1};
    auto R = Reagent{"R", 1};

    //Define Covid Reactions
    const std::initializer_list<reaction> reactions = {
            reaction(LHS {{S,I}} >>= {{E,I}, beta/Nstart}), // susceptible becomes exposed through infectious
            reaction(LHS  {{E}} >>= {{I}, alpha}),// exposed becomes infectious
            reaction(LHS  {{I}}>>= {{{R.name, R.volume}}, gamma}), // infectious becomes removed
            reaction(LHS {{I}} >>= {{H}, kappa}), // infectious becomes hospitalized
            reaction(LHS {{H.name, H.volume}} >>= {{R}, tau})    // hospitalized becomes removed
    };;

    //Construct the Covid Simulator
    Simulator CovidSimulator {reactions, CovidStates};

    //Run the Covid MonitoredSimulation that takes in our function
    CovidSimulator.MonitoredSimulation(100, monitorState<double>);
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
//Computed average hospitalized H for N_NJ = 73.8249 and N_DK = 723.108
//Requirement 10
void runBenchmarks(){
    benchmarking::runBenchmark(1,[=]()->void{
        //Initialize the needed constants
        constexpr int simulatorCount = 10;
        const double simTime = 100;
        const double N = 10000;

        //Initialize the futures vector
        std::vector<std::future<std::vector<reaction::state>>> futures;

        std::cout << "Starting multi-core simulation..." << std::endl;

        //Launch our processors
        for(int i = 0; i < simulatorCount; ++i){
            std::cout << "launching nr: " << i << std::endl;
            auto future = std::async(std::launch::async, performSimulationCovid, simTime, N);
            futures.push_back(std::move(future));
        }

        //Requirement 10, mean calculations
        futureMean("H",std::move(futures));
    });

    //Benchmark for single-core performance
    //Requirement 10
    std::cout << "----- New Benchmarks -----" << std::endl;
    benchmarking::runBenchmark(1,[=]()->void{
        //Initialize the needed constants
        constexpr int simulatorCount = 10;
        const double simTime = 100;
        const double N = 10000;
        double SCMin = 0.0;

        //Launch the single-cores processes
        std::cout << "Starting single-core benchmark simulation..." << std::endl;
        for(int i = 0; i < simulatorCount; i++){
            auto result = performSimulationCovid(simTime, N);
            SCMin += meanCalculator("H",result);
        }

        //Requirement 10, mean calculations
        std::cout << "mean H: " << SCMin/simulatorCount << std::endl;
    });
}

int main(){
    //Requirement 5.1
    //performSimulationABC();

    //Requirement 5.2
    //performSimulationCR(100);

    //Requirement 5.3
    //auto a = performSimulationCovid(100,10000);

    //Requirement 7
    //performSimulationRequirementSeven();

    //Requirement 10
    //runBenchmarks();
    return 0;
}
