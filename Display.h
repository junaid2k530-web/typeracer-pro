#pragma once
#include "PerformanceTracker.h"
#include <string>

// All console output/formatting functions live here
void displayBanner();
void displayRoundHeader(int roundNum, const std::string& difficulty);
void displayStats(const PerformanceTracker* tracker);
void displayFinalSummary(const PerformanceTracker* tracker, const std::string& playerName);
