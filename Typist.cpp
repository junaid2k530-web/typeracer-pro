#include "Typist.h"
#include <algorithm>
#include <vector>

Typist::Typist(const std::string& name) : name(name), wpm(0), accuracy(0) {}

void Typist::startTimer() {
    startTime = std::chrono::steady_clock::now();
}

void Typist::stopTimer() {
    endTime = std::chrono::steady_clock::now();
}

double Typist::getElapsedSeconds() const {
    return std::chrono::duration<double>(endTime - startTime).count();
}

std::string Typist::getName() const { return name; }
double Typist::getWPM() const { return wpm; }
double Typist::getAccuracy() const { return accuracy; }

// Counts errors as the edit distance (Levenshtein distance) between what was
// typed and the target: the fewest single-character insertions, deletions or
// substitutions needed to turn one into the other. Unlike a position-by-position
// comparison, one missed letter only costs 1 error instead of shifting
// everything after it out of line.
int Typist::countErrors(const std::string& typed, const std::string& target) {
    const size_t n = typed.size(), m = target.size();
    std::vector<int> prev(m + 1), curr(m + 1);
    for (size_t j = 0; j <= m; j++) prev[j] = (int)j;   // empty typed -> j insertions
    for (size_t i = 1; i <= n; i++) {
        curr[0] = (int)i;                                // i deletions -> empty target
        for (size_t j = 1; j <= m; j++) {
            int cost = (typed[i - 1] == target[j - 1]) ? 0 : 1;
            curr[j] = std::min({ prev[j] + 1,            // delete a typed char
                                 curr[j - 1] + 1,        // insert a missing char
                                 prev[j - 1] + cost });  // match or substitute
        }
        std::swap(prev, curr);
    }
    return prev[m];
}
