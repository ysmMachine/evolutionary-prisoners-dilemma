#include "Game.hpp"

#include <vector>

Game::Game(PayoffMatrix payoffMatrix, int roundsPerMatch)
    : payoffMatrix(payoffMatrix),
      roundsPerMatch(roundsPerMatch) {
}

MatchResult Game::play(Player& playerA, Player& playerB, std::mt19937& rng) const {
    playerA.getStrategy().reset();
    playerB.getStrategy().reset();

    std::vector<Action> historyA;
    std::vector<Action> historyB;
    historyA.reserve(roundsPerMatch);
    historyB.reserve(roundsPerMatch);

    MatchResult result;

    for (int round = 0; round < roundsPerMatch; ++round) {
        const Action actionA = playerA.getStrategy().decideAction(round, historyA, historyB, rng);
        const Action actionB = playerB.getStrategy().decideAction(round, historyB, historyA, rng);

        const int scoreA = payoffMatrix.scoreFor(actionA, actionB);
        const int scoreB = payoffMatrix.scoreFor(actionB, actionA);

        result.playerAScore += scoreA;
        result.playerBScore += scoreB;

        historyA.push_back(actionA);
        historyB.push_back(actionB);
    }

    playerA.addScore(result.playerAScore);
    playerB.addScore(result.playerBScore);
    return result;
}
