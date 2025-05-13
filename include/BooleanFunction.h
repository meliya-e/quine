#ifndef BOOLEAN_FUNCTION_H
#define BOOLEAN_FUNCTION_H

#include <vector>
#include <string>
#include <set>
#include <map>

class BooleanFunction {
public:
    BooleanFunction(int numVariables);
    
    // Add minterms and don't cares
    void addMinterm(int minterm);
    void addDontCare(int dontCare);
    
    // Get minterms and don't cares
    const std::vector<int>& getMinterms() const;
    const std::vector<int>& getDontCares() const;
    
    // Get number of variables
    int getNumVariables() const;
    
    // Convert number to binary string
    std::string toBinaryString(int num) const;
    
    // Count number of 1s in binary representation
    int countOnes(int num) const;
    
    // Convert binary string to boolean expression
    std::string toBooleanExpression(const std::string& binary) const;

private:
    int numVariables;
    std::vector<int> minterms;
    std::vector<int> dontCares;
};

#endif // BOOLEAN_FUNCTION_H 