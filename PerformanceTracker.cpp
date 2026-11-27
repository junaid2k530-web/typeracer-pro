#include "PerformanceTracker.h"
#include <numeric>
#include <algorithm>

void PerformanceTracker::recordRound(double wpm, double accuracy) {
    history.push_back({wpm, accuracy});
}

double PerformanceTracker::getAverageWPM() const {
    if (history.empty()) return 0;
    double sum = 0;
    for (const auto& r : history) sum += r.wpm;
    return sum / history.size();
}

double PerformanceTracker::getAverageAccuracy() const {
    if (history.empty()) return 0;
    double sum = 0;
    for (const auto& r : history) sum += r.accuracy;
    return sum / history.size();
}

double PerformanceTracker::getBestWPM() const {
    if (history.empty()) return 0;
    double best = 0;
    for (const auto& r : history)
        if (r.wpm > best) best = r.wpm;
    return best;
}

double PerformanceTracker::getLatestWPM() const {
    return history.empty() ? 0 : history.back().wpm;
}

double PerformanceTracker::getLatestAccuracy() const {
    return history.empty() ? 0 : history.back().accuracy;
}

int PerformanceTracker::getTotalRounds() const {
    return (int)history.size();
}

double PerformanceTracker::getWPMTrend() const {
    if (history.size() < 2) return 0;
    // Compare last round to first round
    return history.back().wpm - history.front().wpm;
}
