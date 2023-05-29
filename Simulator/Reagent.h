//
// Created by Daniel Nykjaer on 27/05/2023.
//

#ifndef EKSAMEN2023_REAGENT_H
#define EKSAMEN2023_REAGENT_H

#include <string>

struct Reagent{
    std::string name;
    double volume{};
    Reagent() = default;
    Reagent(std::string  name, double volume): name(std::move(name)), volume(volume) {}

    friend std::ostream& operator<<(std::ostream& os, const Reagent& reagent) {
        os << "" << reagent.name << " (" << reagent.volume << ") ";
        return os;
    }

    Reagent& operator=(const Reagent& other) {
        //self-assignment, no action required
        if (this == &other) {
            return *this;
        }
        this->volume = other.volume;
        return *this;
    }

    Reagent operator+=(const Reagent& other){
        this->volume += other.volume;
        return *this;
    }
};

#endif //EKSAMEN2023_REAGENT_H
