#include "Player.h"
#include <iostream>
#include <string>

Player::Player(const std::string& name) : Typist(name) {}

bool Player::type(const std::string& targetText) {
    std::cout << "\n=== YOUR TURN, " << name << " ===\n";
    std::cout << "Type this text:\n>> " << targetText << "\n\n";
    std::cout << "Press ENTER when ready, then start typing...\n";
    std::cin.ignore();

    std::string typed;
    startTimer();
    std::getline(std::cin, typed);
    stopTimer();

    double elapsed = getElapsedSeconds();
    // Standard: 5 characters = 1 word. Count what was actually typed,
    // so typing half the text doesn't earn credit for all of it.
    double wordCount = typed.size() / 5.0;
    wpm = (wordCount / elapsed) * 60.0;

    int errors = countErrors(typed, targetText);
    accuracy = (1.0 - (double)errors / targetText.size()) * 100.0;
    if (accuracy < 0) accuracy = 0;

    std::cout << "\nYou typed: " << typed << "\n";
    std::cout << "Time: " << elapsed << "s | WPM: " << (int)wpm
              << " | Accuracy: " << (int)accuracy << "%\n";

    return typed == targetText;
}
