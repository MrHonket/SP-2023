//
// Created by Daniel Nykjaer on 27/05/2023.
//

#include "Simulator.h"
#include <iomanip>
#include <fstream>
#include <graphviz/gvc.h>

std::vector<reaction::state> Simulator::runSimulation(double endTime){
    //History works as a monitor, storing data for all the states passed during simulation
    //Requirement 7
    std::vector<reaction::state> History;

    //Produces a dotfile or the loaded reactions
    generateDotFile(reactions);

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
        std::cout << *this << fastestReaction.first;
    }
    return History;
}

void Simulator::generateDotFile(const std::vector<reaction>& vector){
    std::ofstream dotFile("reaction_graph.dot");
    dotFile << "digraph Reaction {\n";

    for(size_t i = 0; i < vector.size(); ++i){
        dotFile << "    R" << i << " [label=\"" << vector[i].lambda << "\", shape=box,style=filled,fillcolor=cyan];\n";
    }

    for (size_t i = 0; i < reactions.size(); ++i) {
        const auto& reaction = reactions[i];
        for (const auto& input : reaction.input) {
            dotFile << "    " << input.name << " -> R" << i << " [color=red];\n";
        }
        for (const auto& output : reaction.output) {
            dotFile << "    R" << i << " -> " << output.name << " [color=green];\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();
}

std::ostream& operator<<(std::ostream& os, const Simulator& value){
    os << value.time <<":  Table Status: | ";
    for (const auto& r : value.state){
        os << r.first <<"(" <<r.second << ")";
    }
    os << "| ";
    return os;
}
