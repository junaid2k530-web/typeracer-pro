#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "AIBot.h"
#include "WordBank.h"
#include "Coach.h"
#include "PerformanceTracker.h"
#include "GameSession.h"

int main() {
    srand(time(nullptr));

    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) name = "Player1";

    // Create all game objects
    Player player(name);
    AIBot  bot("RoboTyper", 40.0);
    WordBank wordBank("words.txt");
    Coach coach;
    PerformanceTracker tracker;

    // Hand them all to GameSession — it runs the show
    GameSession session(&player, &bot, &wordBank, &coach, &tracker);
    session.start();

    return 0;
}
