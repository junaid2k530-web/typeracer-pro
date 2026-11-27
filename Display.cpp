#include "Display.h"
#include <iostream>
#include <iomanip>

void displayBanner() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║        TypeRacer Pro  v1.0           ║\n";
    std::cout << "║     Race the AI. Improve your WPM.   ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "\n";
}

void displayRoundHeader(int roundNum, const std::string& difficulty) {
    std::cout << "\n";
    std::cout << "┌──────────────────────────────────────┐\n";
    std::cout << "│  Round " << std::setw(3) << roundNum
              << "   |   Difficulty: " << std::setw(6) << std::left
              << difficulty << "     │\n";
    std::cout << "└──────────────────────────────────────┘\n";
}

void displayStats(const PerformanceTracker* tracker) {
    if (tracker->getTotalRounds() == 0) return;

    std::cout << "\n--- Session Stats ---\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Rounds played : " << tracker->getTotalRounds() << "\n";
    std::cout << "Latest WPM    : " << tracker->getLatestWPM() << "\n";
    std::cout << "Latest Acc    : " << tracker->getLatestAccuracy() << "%\n";
    std::cout << "Average WPM   : " << tracker->getAverageWPM() << "\n";
    std::cout << "Best WPM      : " << tracker->getBestWPM() << "\n";

    double trend = tracker->getWPMTrend();
    std::cout << "Trend         : " << (trend >= 0 ? "+" : "") << trend << " WPM";
    if      (trend > 5)  std::cout << "  (improving!)";
    else if (trend < -5) std::cout << "  (slipping...)";
    else                 std::cout << "  (steady)";
    std::cout << "\n---------------------\n\n";
}

void displayFinalSummary(const PerformanceTracker* tracker, const std::string& playerName) {
    std::cout << "\n╔══════════════════════════════════════╗\n";
    std::cout << "║          FINAL SUMMARY               ║\n";
    std::cout << "║  Player: " << std::setw(28) << std::left << playerName << "║\n";
    std::cout << "╠══════════════════════════════════════╣\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "║  Total Rounds : " << std::setw(21) << tracker->getTotalRounds() << "║\n";
    std::cout << "║  Best WPM     : " << std::setw(21) << tracker->getBestWPM()     << "║\n";
    std::cout << "║  Avg WPM      : " << std::setw(21) << tracker->getAverageWPM()  << "║\n";
    std::cout << "║  Avg Accuracy : " << std::setw(20) << tracker->getAverageAccuracy() << "%" << "║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "Thanks for playing TypeRacer Pro!\n\n";
}
