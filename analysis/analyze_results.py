"""
Analyse your own TypeRacer Pro sessions.

Every round you play is saved to results.csv (next to the game).
Run this from the game folder:   python analysis/analyze_results.py

It prints a summary per difficulty and saves two charts:
  - learning_curve.png:   net WPM over rounds, to see if you're improving
  - speed_vs_accuracy.png: does typing faster cost you accuracy?

Needs: pandas, matplotlib   (pip install pandas matplotlib)
"""

import sys
import pandas as pd
import matplotlib
matplotlib.use("Agg")            # save charts to files, no window needed
import matplotlib.pyplot as plt

path = sys.argv[1] if len(sys.argv) > 1 else "results.csv"
try:
    df = pd.read_csv(path)
except FileNotFoundError:
    sys.exit(f"No {path} found. Play a few rounds first.")

# Drop rounds where the timer went wrong (e.g. Enter pressed instantly)
df = df[(df["wpm"] > 0) & (df["wpm"] < 250)]
if df.empty:
    sys.exit("No usable rounds in the file yet.")

# Number the rounds across all sessions in the order they were played
df["attempt"] = range(1, len(df) + 1)

print(f"Rounds analysed: {len(df)}\n")
summary = df.groupby("difficulty").agg(
    rounds=("won", "size"),
    win_rate=("won", "mean"),
    avg_net_wpm=("net_wpm", "mean"),
    avg_accuracy=("accuracy", "mean"),
)
print(summary.round(2).to_string(), "\n")

# Correlation between speed and accuracy (near 0 = no trade-off, negative = faster means sloppier)
if len(df) >= 3:
    r = df["wpm"].corr(df["accuracy"])
    print(f"Speed vs accuracy correlation: {r:.2f}")

# Chart 1: learning curve with a rolling average to smooth out noisy rounds
plt.figure(figsize=(7, 4))
plt.plot(df["attempt"], df["net_wpm"], "o", alpha=0.4, label="each round")
plt.plot(df["attempt"], df["net_wpm"].rolling(5, min_periods=1).mean(), label="5-round average")
plt.xlabel("Round")
plt.ylabel("Net WPM (speed x accuracy)")
plt.title("Learning curve")
plt.legend()
plt.tight_layout()
plt.savefig("learning_curve.png", dpi=120)

# Chart 2: speed vs accuracy
plt.figure(figsize=(5, 4))
plt.scatter(df["wpm"], df["accuracy"], alpha=0.6)
plt.xlabel("WPM")
plt.ylabel("Accuracy (%)")
plt.title("Speed vs accuracy")
plt.tight_layout()
plt.savefig("speed_vs_accuracy.png", dpi=120)

print("Saved learning_curve.png and speed_vs_accuracy.png")
