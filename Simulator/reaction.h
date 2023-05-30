//
// Created by Daniel Nykjaer on 27/05/2023.
//

#ifndef EKSAMEN2023_REACTION_H
#define EKSAMEN2023_REACTION_H

#include <utility>
#include <random>
#include "Helper.h"
class reaction {
private:
    reaction(const std::vector<Reagent>& input, const std::vector<Reagent>& output, double lambda) : output(output), input(input), lambda(lambda) {}
    friend reaction create(const std::vector<Reagent>& input, const std::vector<Reagent>& output, double lambda);
public:
    using state = STable<double>;
    double lambda;
    std::vector<Reagent> input{};
    std::vector<Reagent> output{};

    //constructors
    reaction(reaction const &reaction) = default;
    reaction(reaction& other) = default;
    reaction(const LHS& lhs, const RHS& rhs, double lambda) : lambda(lambda), input(lhs.input), output(rhs.output){}
    reaction(const std::initializer_list<Reagent>& lhs, const std::initializer_list<Reagent>& rhs, double lambda) : lambda(lambda), input(lhs),output(rhs){}

    //Move assignment needed for sort in Simulator
    reaction& operator=(reaction&& other) noexcept{
        this->output = std::move(other.output);
        this->input = std::move(other.input);
        this->lambda = other.lambda;
        return *this;
    }

    [[nodiscard]] double compute_delay(reaction::state& state);
    [[nodiscard]] bool canBeSatisfied(reaction::state& state) const;
    void doReaction(reaction::state& state);

    //Requirement 2 - Printing of reaction
    friend std::ostream & operator << (std::ostream& os, const reaction& value);
};

#endif //EKSAMEN2023_REACTION_H
