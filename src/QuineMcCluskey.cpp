#include "QuineMcCluskey.h"
#include <algorithm>
#include <iostream>

QuineMcCluskey::QuineMcCluskey(const BooleanFunction& func) : function(func) {}

bool QuineMcCluskey::canCombine(const std::string& a, const std::string& b, std::string& result) const {
    int differences = 0;
    result = a;
    
    for (size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) {
            differences++;
            result[i] = '-';
        }
        if (differences > 1) return false;
    }
    
    return differences == 1;
}

std::map<int, std::vector<Implicant>> QuineMcCluskey::groupByOnes(const std::vector<int>& terms) {
    std::map<int, std::vector<Implicant>> groups;
    
    for (int term : terms) {
        int ones = function.countOnes(term);
        std::string binary = function.toBinaryString(term);
        groups[ones].emplace_back(std::vector<int>{term}, binary);
    }
    
    return groups;
}

void QuineMcCluskey::combineGroups(std::map<int, std::vector<Implicant>>& groups) {
    std::map<int, std::vector<Implicant>> newGroups;
    std::set<std::string> combined;
    
    for (auto it = groups.begin(); it != std::prev(groups.end()); ++it) {
        for (auto& imp1 : it->second) {
            for (auto& imp2 : std::next(it)->second) {
                std::string result;
                if (canCombine(imp1.binary, imp2.binary, result)) {
                    std::vector<int> combinedMinterms = imp1.minterms;
                    combinedMinterms.insert(combinedMinterms.end(), 
                                          imp2.minterms.begin(), 
                                          imp2.minterms.end());
                    
                    if (combined.find(result) == combined.end()) {
                        newGroups[it->first].emplace_back(combinedMinterms, result);
                        combined.insert(result);
                    }
                    
                    imp1.used = true;
                    imp2.used = true;
                }
            }
        }
    }
    
    // Add unused implicants to prime implicants
    for (const auto& group : groups) {
        for (const auto& imp : group.second) {
            if (!imp.used) {
                primeImplicants.push_back(imp);
            }
        }
    }
    
    if (!newGroups.empty()) {
        combineGroups(newGroups);
    }
}

std::vector<Implicant> QuineMcCluskey::findPrimeImplicants() {
    primeImplicants.clear();
    
    // Combine minterms and don't cares
    std::vector<int> allTerms = function.getMinterms();
    allTerms.insert(allTerms.end(), 
                   function.getDontCares().begin(), 
                   function.getDontCares().end());
    
    // Group terms by number of ones
    auto groups = groupByOnes(allTerms);
    
    // Combine groups
    combineGroups(groups);
    
    return primeImplicants;
}

bool QuineMcCluskey::isEssential(const Implicant& imp, const std::vector<Implicant>& others) const {
    for (int minterm : imp.minterms) {
        bool covered = false;
        for (const auto& other : others) {
            if (std::find(other.minterms.begin(), other.minterms.end(), minterm) != other.minterms.end()) {
                covered = true;
                break;
            }
        }
        if (!covered) return true;
    }
    return false;
}

std::vector<Implicant> QuineMcCluskey::findEssentialPrimeImplicants() {
    std::vector<Implicant> essential;
    std::vector<Implicant> nonEssential;
    
    for (const auto& imp : primeImplicants) {
        std::vector<Implicant> others;
        for (const auto& other : primeImplicants) {
            if (&other != &imp) {
                others.push_back(other);
            }
        }
        
        if (isEssential(imp, others)) {
            essential.push_back(imp);
        } else {
            nonEssential.push_back(imp);
        }
    }
    
    return essential;
}

std::string QuineMcCluskey::getMinimizedExpression() {
    auto essential = findEssentialPrimeImplicants();
    std::string result;
    
    for (size_t i = 0; i < essential.size(); ++i) {
        result += function.toBooleanExpression(essential[i].binary);
        if (i < essential.size() - 1) {
            result += " + ";
        }
    }
    
    return result;
} 