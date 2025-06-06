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

bool QuineMcCluskey::coversMinterm(const Implicant& imp, int minterm) const {
    std::string mintermBinary = function.toBinaryString(minterm);
    for (size_t i = 0; i < imp.binary.length(); ++i) {
        if (imp.binary[i] != '-' && imp.binary[i] != mintermBinary[i]) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<bool>> QuineMcCluskey::buildQuineTable(const std::vector<Implicant>& implicants) {
    std::vector<std::vector<bool>> table(implicants.size(), 
                                       std::vector<bool>(function.getMinterms().size(), false));
    
    for (size_t i = 0; i < implicants.size(); ++i) {
        for (size_t j = 0; j < function.getMinterms().size(); ++j) {
            table[i][j] = coversMinterm(implicants[i], function.getMinterms()[j]);
        }
    }
    
    return table;
}

std::vector<std::set<int>> QuineMcCluskey::findPetrickTerms(const std::vector<std::vector<bool>>& table) {
    std::vector<std::set<int>> petrickTerms;
    
    // For each minterm (column)
    for (size_t j = 0; j < table[0].size(); ++j) {
        std::set<int> term;
        // Find all implicants that cover this minterm
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i][j]) {
                term.insert(i);
            }
        }
        if (!term.empty()) {
            petrickTerms.push_back(term);
        }
    }
    
    return petrickTerms;
}

std::vector<Coverage> QuineMcCluskey::findMinimalCoverages(const std::vector<std::set<int>>& petrickTerms) {
    std::vector<Coverage> coverages;
    std::vector<std::set<int>> currentCoverages;
    
    // Start with the first term
    for (int imp : petrickTerms[0]) {
        currentCoverages.push_back({imp});
    }
    
    // For each subsequent term
    for (size_t i = 1; i < petrickTerms.size(); ++i) {
        std::vector<std::set<int>> newCoverages;
        
        // For each current coverage
        for (const auto& coverage : currentCoverages) {
            // For each implicant in the current term
            for (int imp : petrickTerms[i]) {
                std::set<int> newCoverage = coverage;
                newCoverage.insert(imp);
                newCoverages.push_back(newCoverage);
            }
        }
        
        currentCoverages = newCoverages;
    }
    
    // Convert to Coverage objects
    for (const auto& coverage : currentCoverages) {
        std::vector<Implicant> imps;
        for (int impIndex : coverage) {
            imps.push_back(primeImplicants[impIndex]);
        }
        coverages.emplace_back(imps);
    }
    
    // Sort by cost
    std::sort(coverages.begin(), coverages.end(), 
              [](const Coverage& a, const Coverage& b) { return a.cost < b.cost; });
    
    return coverages;
}

std::vector<Implicant> QuineMcCluskey::findMinimalCoverage() {
    auto essential = findEssentialPrimeImplicants();
    
    // Build Quine's table
    auto table = buildQuineTable(essential);
    
    // Find Petrick's terms
    auto petrickTerms = findPetrickTerms(table);
    
    // Find minimal coverages
    std::vector<Coverage> coverages;
    std::vector<std::set<int>> currentCoverages;
    
    // Start with the first term
    for (int imp : petrickTerms[0]) {
        currentCoverages.push_back({imp});
    }
    
    // For each subsequent term
    for (size_t i = 1; i < petrickTerms.size(); ++i) {
        std::vector<std::set<int>> newCoverages;
        
        // For each current coverage
        for (const auto& coverage : currentCoverages) {
            // For each implicant in the current term
            for (int imp : petrickTerms[i]) {
                std::set<int> newCoverage = coverage;
                newCoverage.insert(imp);
                newCoverages.push_back(newCoverage);
            }
        }
        
        currentCoverages = newCoverages;
    }
    
    // Convert to Coverage objects using essential implicants
    for (const auto& coverage : currentCoverages) {
        std::vector<Implicant> imps;
        for (int impIndex : coverage) {
            imps.push_back(essential[impIndex]);
        }
        coverages.emplace_back(imps);
    }
    
    // Sort by cost
    std::sort(coverages.begin(), coverages.end(), 
              [](const Coverage& a, const Coverage& b) { return a.cost < b.cost; });
    
    // Return the first (minimal) coverage
    if (!coverages.empty()) {
        return coverages[0].implicants;
    }
    
    return essential;
}

std::string QuineMcCluskey::getMinimizedExpression() {
    auto minimalCoverage = findMinimalCoverage();
    std::string result;
    
    for (size_t i = 0; i < minimalCoverage.size(); ++i) {
        result += function.toBooleanExpression(minimalCoverage[i].binary);
        if (i < minimalCoverage.size() - 1) {
            result += " + ";
        }
    }
    
    return result;
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