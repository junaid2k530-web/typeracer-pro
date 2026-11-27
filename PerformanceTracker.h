#pragma once
#include <vector>

// Records per-round stats; raw data is PRIVATE
// Only computed results (average, best, trend) are exposed
class PerformanceTracker {
private:
    struct RoundData {
        double wpm;
        double accuracy;
    };

    std::vector<RoundData> history;   // PRIVATE - raw data hidden

public:
    void recordRound(double wpm, double accuracy);

    // Public computed results only
    double getAverageWPM()       const;
    double getAverageAccuracy()  const;
    double getBestWPM()          const;
    double getLatestWPM()        const;
    double getLatestAccuracy()   const;
    int    getTotalRounds()      const;

    // Trend: positive = improving, negative = declining
    double getWPMTrend() const;
};
