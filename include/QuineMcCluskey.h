#ifndef QUINE_MCCLUSKEY_H
#define QUINE_MCCLUSKEY_H

#include "BooleanFunction.h"
#include <vector>
#include <string>
#include <map>

struct Implicant {
    std::vector<int> minterms;
    std::string binary;
    bool used;
    
    Implicant(const std::vector<int>& m, const std::string& b) 
        : minterms(m), binary(b), used(false) {}
};

class QuineMcCluskey {
public:
    QuineMcCluskey(const BooleanFunction& func);
    
    // Find prime implicants
    std::vector<Implicant> findPrimeImplicants();
    
    // Find essential prime implicants
    std::vector<Implicant> findEssentialPrimeImplicants();
    
    // Get minimized expression
    std::string getMinimizedExpression();

private:
    BooleanFunction function;
    std::vector<Implicant> primeImplicants;
    
    // Helper functions
    bool canCombine(const std::string& a, const std::string& b, std::string& result) const;
    std::map<int, std::vector<Implicant>> groupByOnes(const std::vector<int>& terms);
    void combineGroups(std::map<int, std::vector<Implicant>>& groups);
    bool isEssential(const Implicant& imp, const std::vector<Implicant>& others) const;
};

#endif // QUINE_MCCLUSKEY_H 