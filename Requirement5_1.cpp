#include "Simulator/Helper.h"
#include "Simulator/reaction.h"
#include "Simulator/Simulator.h"

void performSimulation(const Reagent& A,const Reagent& B,const Reagent& C){
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

void runFirstSimulation(){
    const Reagent A = Reagent{"A", 100};
    const Reagent B = Reagent{"B", 0};
    const Reagent C = Reagent{"C", 1};
    performSimulation(A,B,C);
}
void runSecondSimulation()
{

    const Reagent A = Reagent{"A", 100};
    const Reagent B = Reagent{"B", 0};
    const Reagent C = Reagent{"C", 2};
    performSimulation(A,B,C);
}

void runThirdSimulation()
{
    const Reagent A = Reagent{"A", 50};
    const Reagent B = Reagent{"B", 50};
    const Reagent C = Reagent{"C", 1};
    performSimulation(A,B,C);
}

int main(){
    runFirstSimulation();
    runSecondSimulation();
    runThirdSimulation();
    return 0;
}
