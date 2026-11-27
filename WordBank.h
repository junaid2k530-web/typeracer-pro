#pragma once
#include <string>
#include <vector>
#include <map>

// Loads words from a file into Easy / Medium / Hard tiers
// Hides all file-loading and filtering logic (abstraction)
class WordBank {
private:
    std::map<std::string, std::vector<std::string>> tiers;

    void loadFromFile(const std::string& filename);
    void loadDefaults(); // Fallback if file not found

public:
    WordBank(const std::string& filename = "words.txt");

    // Returns 'count' random words from given tier ("Easy"/"Medium"/"Hard")
    std::vector<std::string> getWords(const std::string& tier, int count) const;

    int getTierSize(const std::string& tier) const;
};
