#pragma once
#include <string>

// Adaptive difficulty coach
// Encapsulates difficulty logic - no one can change difficulty directly
class Coach {
private:
    int difficultyLevel;    // 0=Easy, 1=Medium, 2=Hard  (PRIVATE - encapsulated)
    int winsInRow;          // Consecutive wins (PRIVATE)
    int lossesInRow;        // Consecutive losses (PRIVATE)
    double lastWPM;
    double lastAccuracy;

    static const int WIN_STREAK_TO_PROMOTE  = 2;  // Wins needed to go up
    static const int LOSS_STREAK_TO_DEMOTE  = 2;  // Losses needed to go down

public:
    Coach();

    // Called after each round to adjust difficulty
    void evaluate(double playerWPM, double accuracy, bool playerWon);

    // Controlled access - read only
    std::string getDifficulty() const;
    int getDifficultyLevel() const;

    // Read last stats
    double getLastWPM() const;
    double getLastAccuracy() const;
};
