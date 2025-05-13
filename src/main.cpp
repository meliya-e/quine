#include "QuineMcCluskey.h"
#include <iostream>
#include <vector>

int main() {
    int numVariables;
    std::cout << "Enter number of variables: ";
    std::cin >> numVariables;
    
    BooleanFunction func(numVariables);
    
    int numMinterms;
    std::cout << "Enter number of minterms: ";
    std::cin >> numMinterms;
    
    std::cout << "Enter minterms (space-separated): ";
    for (int i = 0; i < numMinterms; ++i) {
        int minterm;
        std::cin >> minterm;
        func.addMinterm(minterm);
    }
    
    int numDontCares;
    std::cout << "Enter number of don't cares: ";
    std::cin >> numDontCares;
    
    if (numDontCares > 0) {
        std::cout << "Enter don't cares (space-separated): ";
        for (int i = 0; i < numDontCares; ++i) {
            int dontCare;
            std::cin >> dontCare;
            func.addDontCare(dontCare);
        }
    }
    
    QuineMcCluskey qm(func);
    
    std::cout << "\nPrime Implicants:" << std::endl;
    auto primeImplicants = qm.findPrimeImplicants();
    for (const auto& imp : primeImplicants) {
        std::cout << func.toBooleanExpression(imp.binary) << std::endl;
    }
    
    std::cout << "\nEssential Prime Implicants:" << std::endl;
    auto essential = qm.findEssentialPrimeImplicants();
    for (const auto& imp : essential) {
        std::cout << func.toBooleanExpression(imp.binary) << std::endl;
    }
    
    std::cout << "\nMinimized Expression:" << std::endl;
    std::cout << qm.getMinimizedExpression() << std::endl;
    
    return 0;
} 