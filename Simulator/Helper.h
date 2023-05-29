//
// Created by Daniel Nykjaer on 27/05/2023.
//

#ifndef EKSAMEN2023_HELPER_H
#define EKSAMEN2023_HELPER_H

#include <iostream>
#include <vector>
#include "Reagent.h"
#include "STable.h"
struct reaction;
struct RHS {
    const std::vector<Reagent> output{};
    const double lambda{};
};

struct LHS {
    const std::vector<Reagent> input{};
    LHS(const std::initializer_list<Reagent>& input) : input(input) {}
    reaction operator>>=(const RHS& rhs);
};

#endif //EKSAMEN2023_HELPER_H
