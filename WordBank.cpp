#include "WordBank.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

WordBank::WordBank(const std::string& filename) {
    loadDefaults(); // Always load defaults first
    loadFromFile(filename); // Override with file if it exists
}

void WordBank::loadDefaults() {
    tiers["Easy"] = {
        "cat", "dog", "run", "sit", "hat", "big", "red", "sun", "top",
        "cup", "sky", "map", "pen", "box", "car", "bus", "hot", "wet",
        "fly", "old", "new", "day", "end", "key", "lip", "mud", "net"
    };
    tiers["Medium"] = {
        "apple", "table", "river", "light", "stone", "cloud", "bread",
        "plant", "floor", "storm", "flame", "brush", "glass", "horse",
        "music", "ocean", "paper", "queen", "radio", "sheep", "tiger",
        "voice", "water", "youth", "zebra", "beach", "crane", "dance"
    };
    tiers["Hard"] = {
        "algorithm", "abstract", "challenge", "database", "eloquent",
        "franchise", "glamorous", "hierarchy", "impulsive", "jealousy",
        "knowledge", "labyrinth", "magnitude", "negotiate", "objective",
        "paramount", "qualified", "rectangle", "structure", "technical",
        "universal", "vibration", "wandering", "xylophone", "yearnings",
        "zeppelin", "beautiful", "conscious", "different", "excellent"
    };
}

void WordBank::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "[WordBank] No words.txt found, using built-in words.\n";
        return;
    }

    std::string line;
    std::string currentTier = "";

    while (std::getline(file, line)) {
        if (line == "Easy:" || line == "Medium:" || line == "Hard:") {
            currentTier = line.substr(0, line.size() - 1);
            tiers[currentTier].clear(); // Override defaults
        } else if (!line.empty() && !currentTier.empty()) {
            tiers[currentTier].push_back(line);
        }
    }

    std::cout << "[WordBank] Loaded words from " << filename << "\n";
}

std::vector<std::string> WordBank::getWords(const std::string& tier, int count) const {
    auto it = tiers.find(tier);
    if (it == tiers.end() || it->second.empty()) {
        return {"the", "quick", "brown", "fox", "jumps"}; // Emergency fallback
    }

    std::vector<std::string> pool = it->second;
    std::vector<std::string> result;

    // Shuffle and pick 'count' words
    for (int i = pool.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(pool[i], pool[j]);
    }

    for (int i = 0; i < count && i < (int)pool.size(); i++) {
        result.push_back(pool[i]);
    }

    return result;
}

int WordBank::getTierSize(const std::string& tier) const {
    auto it = tiers.find(tier);
    return (it != tiers.end()) ? it->second.size() : 0;
}
