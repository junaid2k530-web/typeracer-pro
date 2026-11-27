#pragma once
#include "Typist.h"

// AI opponent - simulates typing at a configurable WPM
class AIBot : public Typist {
private:
    double targetWPM;   // The WPM the bot aims for
    double errorRate;   // Probability of making a typo (0.0 to 1.0)

public:
    AIBot(const std::string& name, double targetWPM, double errorRate = 0.02);

    void setTargetWPM(double wpm);

    // Overrides Typist::type() with simulated speed + errors
    bool type(const std::string& targetText) override;
};
