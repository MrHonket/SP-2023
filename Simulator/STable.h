//
// Created by Daniel Nykjaer on 27/05/2023.
//

#ifndef EKSAMEN2023_STABLE_H
#define EKSAMEN2023_STABLE_H

#include <iostream>
#include <stdexcept>
#include <optional>
#include <unordered_map>
#include <string>

//Implementation of generic symbol-table called STable
//Requirement 3

template<typename T>
class STable{
public:
    std::unordered_map<std::string, T> SymbolTable;

    void store(const std::string &key, T value){
        SymbolTable.emplace(key,value);
    }

    bool contains(const std::string &key){
        return (SymbolTable.find(key) != SymbolTable.end());
    }

    void update(const std::string &symbol, T value) {
        const auto& found = SymbolTable.find(symbol);
        if(found != SymbolTable.end()){
            (*found).second = value;
        } else{
            store(symbol, value);
        }
    }

    std::optional<T> lookup(const std::string &key){
        try {
            if(contains(key))
                return SymbolTable.find(key)->second;
            return std::nullopt;
        } catch (...) {
            std::cout << "No object for key found";
        }
    }

    [[nodiscard]] size_t size() const {
        return SymbolTable.size();
    }

    auto begin() { return SymbolTable.begin(); }
    auto end() { return SymbolTable.end(); }
    auto begin() const { return SymbolTable.begin(); }
    auto end() const { return SymbolTable.end(); }
};

#endif //EKSAMEN2023_STABLE_H
