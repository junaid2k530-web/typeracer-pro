"""
Does the difficulty coach actually put players at the right level?

The game's coach moves difficulty up after 2 wins in a row and down after
2 losses in a row. This script simulates many players of different skill
and compares that rule against two alternatives:

  - fixed_easy:  never changes difficulty (a baseline)
  - streak:      the rule the game uses (2 wins up / 2 losses down)
  - skill_match: picks the hardest level whose bot is slower than the
                 player's average net WPM over their last 3 rounds

It measures, for each rule:
  - win rate:        a good coach keeps this near 50% (challenging but fair)
  - right level %:   share of rounds played at the level that best fits the
                     player's true skill
  - level switches:  how often difficulty changes (lots of switching = unstable)

Everything here is simulated. The player model is a simple assumption
(see PLAYER MODEL below), not data from real players.
"""

import random
import statistics

# --- Game settings (taken from GameSession.cpp and AIBot.h) ---
BOT_WPM = [30, 50, 70]           # bot speed at Easy, Medium, Hard
BOT_ACCURACY = 0.98              # bot error rate is 2%
BOT_NET = [w * BOT_ACCURACY for w in BOT_WPM]   # bot net WPM per level
LEVELS = ["Easy", "Medium", "Hard"]

# --- PLAYER MODEL (assumptions) ---
ROUND_NOISE = 6.0        # a player's net WPM varies by about +/-6 between rounds
PRACTICE_GAIN = 0.15     # true skill improves by 0.15 net WPM per round played
ROUNDS = 40              # rounds per simulated session
PLAYERS_PER_SKILL = 2000 # simulated players for each starting skill
SKILLS = [20, 35, 50, 65, 80]    # starting true net WPM of the players


def right_level(skill):
    """The level whose bot net WPM is closest to the player's true skill."""
    gaps = [abs(skill - b) for b in BOT_NET]
    return gaps.index(min(gaps))


def fixed_easy(level, history, state):
    """Baseline: always Easy."""
    return 0


def streak(level, history, state):
    """The game's rule: 2 wins in a row -> up, 2 losses in a row -> down."""
    won = history[-1][1]
    if won:
        state["wins"] += 1
        state["losses"] = 0
        if state["wins"] >= 2 and level < 2:
            state["wins"] = 0
            return level + 1
    else:
        state["losses"] += 1
        state["wins"] = 0
        if state["losses"] >= 2 and level > 0:
            state["losses"] = 0
            return level - 1
    return level


def skill_match(level, history, state):
    """Hardest level whose bot is slower than the player's recent average."""
    recent = [net for net, _ in history[-3:]]
    avg = statistics.mean(recent)
    best = 0
    for lvl, bot in enumerate(BOT_NET):
        if bot <= avg:
            best = lvl
    return best


def run_session(start_skill, policy, rng):
    """Simulate one player's session; return (win rate, right-level share, switches)."""
    skill = start_skill
    level = 0                    # every session starts on Easy, like the game
    state = {"wins": 0, "losses": 0}
    history = []                 # list of (player net WPM, won?) per round
    wins = right = switches = 0

    for _ in range(ROUNDS):
        player_net = rng.gauss(skill, ROUND_NOISE)
        won = player_net > BOT_NET[level]
        wins += won
        right += (level == right_level(skill))
        history.append((player_net, won))

        new_level = policy(level, history, state)
        switches += (new_level != level)
        level = new_level
        skill += PRACTICE_GAIN

    return wins / ROUNDS, right / ROUNDS, switches


def main():
    rng = random.Random(42)      # fixed seed so results are reproducible
    policies = {"fixed_easy": fixed_easy, "streak": streak, "skill_match": skill_match}

    print(f"{'start skill':>11} | {'policy':<11} | {'win rate':>8} | {'right level':>11} | {'switches':>8}")
    print("-" * 62)
    for skill in SKILLS:
        for name, policy in policies.items():
            results = [run_session(skill, policy, rng) for _ in range(PLAYERS_PER_SKILL)]
            win = statistics.mean(r[0] for r in results)
            right = statistics.mean(r[1] for r in results)
            sw = statistics.mean(r[2] for r in results)
            print(f"{skill:>11} | {name:<11} | {win:>7.0%} | {right:>10.0%} | {sw:>8.1f}")
        print("-" * 62)


if __name__ == "__main__":
    main()
