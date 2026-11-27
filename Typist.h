#pragma once
#include <string>
#include <chrono>

// Abstract base class for all typists (Player & AIBot)
class Typist {
protected:
    std::string name;
    double wpm;        // Words per minute
    double accuracy;   // Accuracy percentage

    // Timing - hidden from subclasses (abstraction)
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    void startTimer();
    void stopTimer();
    double getElapsedSeconds() const;

public:
    Typist(const std::string& name);
    virtual ~Typist() = default;

    // Pure virtual - must be overridden
    virtual bool type(const std::string& targetText) = 0;

    // Getters
    std::string getName() const;
    double getWPM() const;
    double getAccuracy() const;

    // Utility
    static int countErrors(const std::string& typed, const std::string& target);
};
