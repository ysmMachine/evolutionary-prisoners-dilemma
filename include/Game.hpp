#pragma once

#include "PayoffMatrix.hpp"
#include "Player.hpp"

#include <random>

struct MatchResult {
    double playerAScore = 0.0;
    double playerBScore = 0.0;
};
class Game {
public:
    Game(PayoffMatrix payoffMatrix, int roundsPerMatch);
    MatchResult play(Player& playerA, Player& playerB, std::mt19937& rng) const;

private:
    PayoffMatrix payoffMatrix;
    int roundsPerMatch;
};
