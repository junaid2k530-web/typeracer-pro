#pragma once
#include "Player.h"
#include "AIBot.h"
#include "WordBank.h"
#include "Coach.h"
#include "PerformanceTracker.h"

// Orchestrates one full game session
// Exposes only start() — all round logic is hidden inside
class GameSession {
private:
    Player* player;
    AIBot*  bot;
    WordBank* wordBank;
    Coach* coach;
    PerformanceTracker* tracker;
    int roundNumber;

    std::string buildRaceText(const std::vector<std::string>& words);
    void logRound(const std::string& difficulty, double pWPM, double pAcc,
                  double pNet, double bNet, bool playerWon);
    void showRoundResult(bool playerWon, double playerWPM, double playerAcc,
                         double botWPM, double botAcc);

public:
    GameSession(Player* player, AIBot* bot, WordBank* wordBank,
                Coach* coach, PerformanceTracker* tracker);

    void start();   // The only public entry point
};
