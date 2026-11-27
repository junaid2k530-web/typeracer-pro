# TypeRacer Pro

A console typing game in C++ where you race an AI bot, with a coach that changes the difficulty as you win or lose. Group project for CS112 (Object-Oriented Programming and Design) at GIKI, Spring 2026.

## How a round works

1. You get a line of 8 words, picked from the word list for your current difficulty (Easy, Medium or Hard).
2. The bot "types" it first, at a speed set by the difficulty (30, 50 or 70 WPM) with a small random error rate. A progress bar shows it typing.
3. You type the same line. The game times you and counts your errors.
4. Whoever has the higher **net WPM** (speed × accuracy) wins the round. You have to get at least half the text right for the round to count.
5. The round is saved to `results.csv`.
6. The coach updates the difficulty, and you see your session stats: average and best WPM, accuracy, and whether you're improving.

**The coach:** two wins in a row moves you up a difficulty level, and two losses in a row moves you down.

## Design

The project was built to practise object-oriented design:

| Class | Job |
|---|---|
| `Typist` | Abstract base class: name, WPM, accuracy, timing, and a pure virtual `type()` |
| `Player`, `AIBot` | Two implementations of `Typist`: one reads your keyboard, the other simulates typing |
| `GameSession` | Runs the rounds. `start()` is its only public method |
| `Coach` | Owns the difficulty level. Nothing else can change it directly |
| `PerformanceTracker` | Keeps per-round history private and only exposes computed stats (average, best, trend) |
| `WordBank` | Built-in word lists per difficulty, which `words.txt` can override |
| `Display` | Banner, headers, stats and final summary |

## Build and run

```
g++ -std=c++17 *.cpp -o typeracer
./typeracer
```
Or with `make`, then `make run`. Run it from this folder so it finds `words.txt`.

## Extensions added after the course (Nov 2026)

After the course I extended the project so it measures typing properly and tests whether the coach actually works. These parts were added later and weren't part of the group coursework.

### 1. Better measurement
- **Errors are now counted with edit distance (Levenshtein).** The old version compared characters position by position, so a single missed letter shifted everything after it and counted as dozens of errors. Now one missed letter costs one error.
- **WPM now counts what you actually typed.** Before, typing half the text still earned credit for all of it.
- **Rounds are decided on net WPM.** Before, accuracy was ignored, so pressing Enter instantly "won" at millions of WPM.

### 2. Every round is logged
Each round is appended to `results.csv` (player, round, difficulty, WPM, accuracy, net WPM, bot net WPM, won).

`analysis/analyze_results.py` reads that file. It prints win rate, net WPM and accuracy for each difficulty, plus the correlation between speed and accuracy, and saves a learning-curve chart and a speed-vs-accuracy chart.

```
pip install pandas matplotlib
python analysis/analyze_results.py
```

### 3. Does the coach put players at the right level?
`analysis/coach_simulation.py` simulates 2,000 players at each of five starting skill levels over 40 rounds, and compares three rules:

- **fixed_easy:** never changes difficulty (a baseline)
- **streak:** the game's rule (2 wins up, 2 losses down)
- **skill_match:** picks the hardest level whose bot is slower than the player's 3-round average

Results (`python analysis/coach_simulation.py`, seed 42):

| Start skill (net WPM) | Rule | Win rate | Rounds at right level | Level switches |
|---|---|---|---|---|
| 35 | streak | 50% | 54% | 17.9 |
| 35 | skill_match | 91% | 73% | 0.1 |
| 50 | streak | 53% | 56% | 16.8 |
| 50 | skill_match | 80% | 81% | 6.1 |
| 65 | streak | 65% | 67% | 11.3 |
| 65 | skill_match | 80% | 40% | 10.2 |

**What this shows:** the streak rule keeps mid-skill players near a 50% win rate, which is what makes a game feel challenging. But it does that by flipping difficulty about every 2 rounds, and it only has the player at the right level about half the time. The skill-match rule is more stable and more often at the right level for most players, but players win far more often, so the game gets easy. Neither rule is better on every measure. A coach that picks the level from recent performance but adds some challenge on purpose could get both.

**Limits:** the player model is an assumption (round-to-round noise of ±6 net WPM, steady improvement of 0.15 net WPM per round), not data from real players. The next step would be to check the model against real `results.csv` logs.
