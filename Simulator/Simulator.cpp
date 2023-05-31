//
// Created by Daniel Nykjaer on 27/05/2023.
//

#include "Simulator.h"
#include <iomanip>
#include <fstream>

//Requirement 2
//Functions that enables outputting the loaded reaction network
void Simulator::printLoadedReactions(){
    for (auto& reaction : reactions) {
        std::cout << reaction;
    }
}

std::vector<reaction::state> Simulator::runSimulation(double endTime){
    std::vector<reaction::state> History;

    //Produces a dotfile of the loaded reactions
    generateDotFile(reactions);

    while (time < endTime){
        std::vector<std::pair<reaction, double>> validReactionTimes{};

        for (auto& reaction :reactions) {
            if (reaction.isDoable(state))
                validReactionTimes.emplace_back(reaction, reaction.computeDelay(state));
        }

        std::sort(validReactionTimes.begin(), validReactionTimes.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });

        auto& fastestReaction = validReactionTimes.front();
        time += fastestReaction.second;
        fastestReaction.first.doReaction(state);
        History.emplace_back(state);

        //remember to comment out for simulations
        //std::cout << *this << fastestReaction.first;
    }
    return History;
}
//Requirement 7
//Added simulation mode that takes in a generic monitor function on call-time
//Does not store the state in history as the requirement specified
void Simulator::monitoredSimulation(double endTime, std::function<void(STable<double>&)> stateMonitor) {
    //Produces a dotfile or the loaded reactions
    generateDotFile(reactions);

    while (time < endTime){
        std::vector<std::pair<reaction, double>> delaysForSatisfiableReactions{};

        for (auto& reaction :reactions) {
            if (reaction.isDoable(state))
                delaysForSatisfiableReactions.emplace_back(reaction, reaction.computeDelay(state));
        }

        std::sort(delaysForSatisfiableReactions.begin(), delaysForSatisfiableReactions.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });

        auto& fastestReaction = delaysForSatisfiableReactions.front();
        time += fastestReaction.second;
        fastestReaction.first.doReaction(state);

        //call the provided stateMonitor function
        stateMonitor(state);
    }
}

//Requirement 2 Dotfile
//Function for producing the .dot needed for graphviz to generate a reaction network
void Simulator::generateDotFile(const std::vector<reaction>& reactions){
    std::ofstream dotFile("reaction_graph.dot");
    dotFile << "digraph Reaction {\n";

    for(size_t i = 0; i < reactions.size(); ++i){
        dotFile << "    R" << i << " [label=\"" << reactions[i].lambda << "\", shape=box,style=filled,fillcolor=cyan];\n";
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
    for (const auto& reagent : value.state){
        os << reagent.first << "(" << reagent.second << ")";
    }
    os << "| ";
    return os;
}
