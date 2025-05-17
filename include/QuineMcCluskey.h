#ifndef QUINE_MCCLUSKEY_H
#define QUINE_MCCLUSKEY_H

#include "BooleanFunction.h"
#include <vector>
#include <string>
#include <map>
#include <set>

struct Implicant {
    std::vector<int> minterms;
    std::string binary;
    bool used;
    
    Implicant(const std::vector<int>& m, const std::string& b) 
        : minterms(m), binary(b), used(false) {}
};

struct Coverage {
    std::vector<Implicant> implicants;
    int cost; // number of literals in the expression
    
    Coverage(const std::vector<Implicant>& imps) : implicants(imps) {
        cost = 0;
        for (const auto& imp : imps) {
            for (char c : imp.binary) {
                if (c != '-') cost++;
            }
        }
    }
};

class QuineMcCluskey {
public:
    QuineMcCluskey(const BooleanFunction& func);
    
    // Find prime implicants
    std::vector<Implicant> findPrimeImplicants();
    
    // Find essential prime implicants
    std::vector<Implicant> findEssentialPrimeImplicants();
    
    // Find minimal coverage using Quine's table
    std::vector<Implicant> findMinimalCoverage();
    
    // Get minimized expression
    std::string getMinimizedExpression();
    
    // Build Quine's table
    std::vector<std::vector<bool>> buildQuineTable(const std::vector<Implicant>& implicants);

private:
    BooleanFunction function;
    std::vector<Implicant> primeImplicants;
    
    // Helper functions
    bool canCombine(const std::string& a, const std::string& b, std::string& result) const;
    std::map<int, std::vector<Implicant>> groupByOnes(const std::vector<int>& terms);
    void combineGroups(std::map<int, std::vector<Implicant>>& groups);
    bool isEssential(const Implicant& imp, const std::vector<Implicant>& others) const;
    
    // Helper functions for minimal coverage
    std::vector<std::set<int>> findPetrickTerms(const std::vector<std::vector<bool>>& table);
    std::vector<Coverage> findMinimalCoverages(const std::vector<std::set<int>>& petrickTerms);
    bool coversMinterm(const Implicant& imp, int minterm) const;
};

#endif // QUINE_MCCLUSKEY_H 