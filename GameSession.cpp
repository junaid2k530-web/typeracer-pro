#include "GameSession.h"
#include "Display.h"
#include <iostream>
#include <sstream>
#include <fstream>

GameSession::GameSession(Player* player, AIBot* bot, WordBank* wordBank,
                         Coach* coach, PerformanceTracker* tracker)
    : player(player), bot(bot), wordBank(wordBank),
      coach(coach), tracker(tracker), roundNumber(0) {}

std::string GameSession::buildRaceText(const std::vector<std::string>& words) {
    std::ostringstream oss;
    for (size_t i = 0; i < words.size(); i++) {
        if (i > 0) oss << " ";
        oss << words[i];
    }
    return oss.str();
}

void GameSession::showRoundResult(bool playerWon, double playerWPM, double playerAcc,
                                   double botWPM, double botAcc) {
    std::cout << "\n========================================\n";
    std::cout << "           ROUND " << roundNumber << " RESULT\n";
    std::cout << "========================================\n";
    std::cout << player->getName() << ": " << (int)playerWPM << " WPM | "
              << (int)playerAcc << "% accuracy\n";
    std::cout << bot->getName()    << ": " << (int)botWPM    << " WPM | "
              << (int)botAcc    << "% accuracy\n";
    std::cout << "----------------------------------------\n";
    std::cout << (playerWon ? ">>> YOU WIN! <<<" : ">>> BOT WINS. Try again! <<<") << "\n";
    std::cout << "========================================\n\n";
}

void GameSession::start() {
    displayBanner();

    bool keepPlaying = true;
    while (keepPlaying) {
        roundNumber++;
        std::string difficulty = coach->getDifficulty();

        displayRoundHeader(roundNumber, difficulty);

        // Get words for this difficulty
        std::vector<std::string> words = wordBank->getWords(difficulty, 8);
        std::string raceText = buildRaceText(words);

        // Adjust bot WPM based on difficulty
        if      (difficulty == "Easy")   bot->setTargetWPM(30);
        else if (difficulty == "Medium") bot->setTargetWPM(50);
        else                             bot->setTargetWPM(70);

        // AI goes first (simulated), then player types
        bot->type(raceText);
        player->type(raceText);

        double pWPM = player->getWPM();
        double pAcc = player->getAccuracy();
        double bWPM = bot->getWPM();
        double bAcc = bot->getAccuracy();

        // Compare net WPM (speed adjusted for accuracy), so typing fast
        // but wrong - or just pressing Enter - can't win a round
        double pNet = pWPM * pAcc / 100.0;
        double bNet = bWPM * bAcc / 100.0;
        // A round only counts if at least half the text was typed correctly
        bool finished = pAcc >= 50.0;
        bool playerWon = finished && pNet > bNet;

        // Record stats
        tracker->recordRound(pWPM, pAcc);

        // Save this round to results.csv so sessions can be analysed later
        logRound(difficulty, pWPM, pAcc, pNet, bNet, playerWon);

        // Coach adjusts difficulty
        coach->evaluate(pWPM, pAcc, playerWon);

        // Show result
        showRoundResult(playerWon, pWPM, pAcc, bWPM, bAcc);
        displayStats(tracker);

        // Ask to play again
        std::cout << "Play another round? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();
        keepPlaying = (choice == 'y' || choice == 'Y');
    }

    displayFinalSummary(tracker, player->getName());
}

// Appends one row per round to results.csv (writes the header if the file is new)
void GameSession::logRound(const std::string& difficulty, double pWPM, double pAcc,
                           double pNet, double bNet, bool playerWon) {
    std::ifstream check("results.csv");
    bool isNew = !check.good();
    check.close();

    std::ofstream out("results.csv", std::ios::app);
    if (!out) return;  // can't write here: skip logging, keep playing
    if (isNew)
        out << "player,round,difficulty,wpm,accuracy,net_wpm,bot_net_wpm,won\n";
    out << player->getName() << "," << roundNumber << "," << difficulty << ","
        << pWPM << "," << pAcc << "," << pNet << "," << bNet << ","
        << (playerWon ? 1 : 0) << "\n";
}
