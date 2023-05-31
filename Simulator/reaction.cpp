//
// Created by Daniel Nykjaer on 27/05/2023.
//
#include "reaction.h"

bool reaction::canBeSatisfied(reaction::state& state) const {
    return std::all_of(input.begin(), input.end(), [&state](const auto& reagentInvolved) {
        auto inputVolume = state.lookup(reagentInvolved.name);
        return inputVolume.has_value() && inputVolume.value() >= reagentInvolved.volume;
    });
}

double reaction::computeDelay(reaction::state& state){
    auto volumeProduct = 1.0;
    for (const auto& i : input){
        volumeProduct *= state.lookup(i.name).value();
    }
    if (volumeProduct == 0) return 0;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::exponential_distribution<> exponentialDistribution(volumeProduct * lambda);
    auto delay =  exponentialDistribution(generator);
    return delay;
}

reaction create(const std::vector<Reagent>& input, const std::vector<Reagent>& output, double lambda) {
    return {input, output, lambda};
}

void reaction::doReaction(reaction::state &state){
    for (const Reagent& i : input) {
        state.update(i.name, state.lookup(i.name).value() - i.volume);
    }
    for (const Reagent& i : output) {
        state.update(i.name, state.lookup(i.name).value_or(0) + i.volume);
    }
};

//Requirement 1
//Overloading of the >>= operator allowing for easier creation of rules
reaction LHS::operator>>=(const RHS& rhs) {
    return create(this->input, rhs.output, rhs.lambda);
}

//Requirement 2 - Printing of reaction
std::ostream& operator<<(std::ostream& os, const reaction& reaction){
    os <<"( ";
    for (const auto& input : reaction.input){
        os << input.name << " ";
    }
    os << "-> " ;
    if(reaction.output.empty())
        os << "Void";

    for (const auto& output : reaction.output) {
        os << output.name << " ";
    }
    os << ")" << std::endl;
    return os;
}
