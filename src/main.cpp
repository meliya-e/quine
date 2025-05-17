#include "QuineMcCluskey.h"
#include <iostream>
#include <vector>
#include <iomanip>

void printQuineTable(const std::vector<std::vector<bool>>& table, 
                    const std::vector<Implicant>& implicants,
                    const std::vector<int>& minterms) {
    std::cout << "\nQuine's Table:" << std::endl;
    std::cout << std::setw(15) << "Implicant";
    for (int m : minterms) {
        std::cout << std::setw(5) << m;
    }
    std::cout << std::endl;
    
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << std::setw(15) << implicants[i].binary;
        for (bool covered : table[i]) {
            std::cout << std::setw(5) << (covered ? "X" : " ");
        }
        std::cout << std::endl;
    }
}

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
    
    // Build and print Quine's table
    auto table = qm.buildQuineTable(essential);
    printQuineTable(table, essential, func.getMinterms());
    
    std::cout << "\nMinimal Coverage:" << std::endl;
    auto minimalCoverage = qm.findMinimalCoverage();
    for (const auto& imp : minimalCoverage) {
        std::cout << func.toBooleanExpression(imp.binary) << std::endl;
    }
    
    std::cout << "\nMinimized Expression:" << std::endl;
    std::cout << qm.getMinimizedExpression() << std::endl;
    
    return 0;
} 