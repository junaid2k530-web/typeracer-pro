#include "AIBot.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>

AIBot::AIBot(const std::string& name, double targetWPM, double errorRate)
    : Typist(name), targetWPM(targetWPM), errorRate(errorRate) {}

void AIBot::setTargetWPM(double wpm) {
    targetWPM = wpm;
}

bool AIBot::type(const std::string& targetText) {
    std::cout << "\n=== AI BOT [" << name << "] is typing... (WPM: " << (int)targetWPM << ") ===\n";

    // Simulate typing time based on WPM
    double wordCount = targetText.size() / 5.0;
    double simulatedTime = (wordCount / targetWPM) * 60.0;

    // Add small random variance (+/- 10%)
    double variance = simulatedTime * 0.1 * ((rand() % 21 - 10) / 100.0);
    simulatedTime += variance;

    startTimer();

    // Simulate typing with a progress bar
    int steps = 20;
    for (int i = 0; i <= steps; i++) {
        std::cout << "\r[";
        for (int j = 0; j < steps; j++) std::cout << (j < i ? '#' : '-');
        std::cout << "] " << (i * 5) << "%" << std::flush;
        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(simulatedTime * 1000 / steps))
        );
    }
    std::cout << "\n";

    stopTimer();
    double elapsed = getElapsedSeconds();
    wpm = (wordCount / elapsed) * 60.0;

    // Build typed string with possible errors
    std::string typed = targetText;
    int errors = 0;
    for (char& c : typed) {
        if ((double)rand() / RAND_MAX < errorRate) {
            c = 'a' + rand() % 26; // Random char substitution
            errors++;
        }
    }

    accuracy = (1.0 - (double)errors / targetText.size()) * 100.0;
    if (accuracy < 0) accuracy = 0;

    std::cout << name << " finished! Time: " << elapsed << "s | WPM: "
              << (int)wpm << " | Accuracy: " << (int)accuracy << "%\n";

    return typed == targetText;
}
