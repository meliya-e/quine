#include "BooleanFunction.h"
#include <algorithm>

BooleanFunction::BooleanFunction(int numVariables) : numVariables(numVariables) {}

void BooleanFunction::addMinterm(int minterm) {
    minterms.push_back(minterm);
    std::sort(minterms.begin(), minterms.end());
}

void BooleanFunction::addDontCare(int dontCare) {
    dontCares.push_back(dontCare);
    std::sort(dontCares.begin(), dontCares.end());
}

const std::vector<int>& BooleanFunction::getMinterms() const {
    return minterms;
}

const std::vector<int>& BooleanFunction::getDontCares() const {
    return dontCares;
}

int BooleanFunction::getNumVariables() const {
    return numVariables;
}

std::string BooleanFunction::toBinaryString(int num) const {
    std::string result;
    for (int i = numVariables - 1; i >= 0; --i) {
        result += ((num >> i) & 1) ? '1' : '0';
    }
    return result;
}

int BooleanFunction::countOnes(int num) const {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

std::string BooleanFunction::toBooleanExpression(const std::string& binary) const {
    std::string result;
    for (int i = 0; i < binary.length(); ++i) {
        if (binary[i] == '1') {
            result += static_cast<char>('A' + i);
        } else if (binary[i] == '0') {
            result += static_cast<char>('A' + i);
            result += "'";
        }
    }
    return result;
} 