//
// Created by Daniel Nykjaer on 27/05/2023.
//

#ifndef EKSAMEN2023_SIMULATOR_H
#define EKSAMEN2023_SIMULATOR_H

#include <functional>
#include "Helper.h"
#include "reaction.h"

//Implementation of the class handling stochastic simulation
//Requirement 4.
class Simulator {
private:
    double time = 0.0;
public:
    std::vector<reaction> reactions;
    reaction::state state;

    Simulator(const std::initializer_list<reaction>& reactions, reaction::state& reagentValues): reactions(reactions), state(reagentValues)
    {};

    friend std::ostream & operator << (std::ostream& s, const Simulator& value);

    std::vector<reaction::state> runSimulation(double endTime);

    void MonitoredSimulation(double endTime, std::function<void(STable<double>&)> stateMonitor);

    void generateDotFile(const std::vector<reaction>& vector);

    void printLoadedReactions();
};

#endif //EKSAMEN2023_SIMULATOR_H
