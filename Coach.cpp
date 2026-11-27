#include "Coach.h"
#include <iostream>

Coach::Coach() : difficultyLevel(0), winsInRow(0), lossesInRow(0),
                 lastWPM(0), lastAccuracy(0) {}

void Coach::evaluate(double playerWPM, double accuracy, bool playerWon) {
    lastWPM = playerWPM;
    lastAccuracy = accuracy;

    if (playerWon) {
        winsInRow++;
        lossesInRow = 0;

        if (winsInRow >= WIN_STREAK_TO_PROMOTE && difficultyLevel < 2) {
            difficultyLevel++;
            winsInRow = 0;
            std::cout << "[Coach] Great work! Bumping difficulty to "
                      << getDifficulty() << ".\n";
        } else {
            std::cout << "[Coach] Nice win! Keep it up.\n";
        }
    } else {
        lossesInRow++;
        winsInRow = 0;

        if (lossesInRow >= LOSS_STREAK_TO_DEMOTE && difficultyLevel > 0) {
            difficultyLevel--;
            lossesInRow = 0;
            std::cout << "[Coach] Let's step back to "
                      << getDifficulty() << " to build confidence.\n";
        } else {
            std::cout << "[Coach] You'll get it! Focus on accuracy first.\n";
        }
    }
}

std::string Coach::getDifficulty() const {
    switch (difficultyLevel) {
        case 0: return "Easy";
        case 1: return "Medium";
        case 2: return "Hard";
        default: return "Easy";
    }
}

int Coach::getDifficultyLevel() const { return difficultyLevel; }
double Coach::getLastWPM() const { return lastWPM; }
double Coach::getLastAccuracy() const { return lastAccuracy; }
