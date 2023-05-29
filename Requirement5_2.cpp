//
// Created by Daniel Nykjaer on 27/05/2023.
//
#include "Simulator/Helper.h"
#include "Simulator/reaction.h"
#include "Simulator/Simulator.h"

void performSimulation(double simtime){
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

int main(){
    performSimulation(100);
    return 0;
}