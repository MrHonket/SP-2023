//
// Created by Daniel Nykjaer on 27/05/2023.
//

#include "Simulator.h"
#include <iomanip>

std::vector<reaction::state> Simulator::runSimulation(double endTime){
    //History works as a monitor, storing data for all the states passed during simulation
    //Requirement 7
    std::vector<reaction::state> History;
    while (time < endTime){
        std::vector<std::pair<reaction, double>> validReactionTimes{};

        for (auto& reaction :reactions) {
            if (reaction.canBeSatisfied(state))
                validReactionTimes.emplace_back(reaction, reaction.compute_delay(state));
        }

        std::sort(validReactionTimes.begin(), validReactionTimes.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });

        auto& fastestReaction = validReactionTimes.front();
        time += fastestReaction.second;
        fastestReaction.first.doReaction(state);
        History.emplace_back(state);

        //Handles pretty printing the actions using the overloaded << operators
        //Requirement 2.a
        //remember to comment out for simulations
        //std::cout << *this << fastestReaction.first;
    }
    return History;
}

std::ostream& operator<<(std::ostream& os, const Simulator& value){
    os << value.time <<":  Table Status: | ";
    for (const auto& r : value.state){
        os << r.first <<"(" <<r.second << ")";
    }
    os << "| ";
    return os;
}
