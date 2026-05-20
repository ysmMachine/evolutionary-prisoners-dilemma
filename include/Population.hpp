#pragma once

#include "Player.hpp"

#include <map>
#include <random>
#include <vector>

struct StrategyStats {
    int count = 0;
    double totalScore = 0.0;
    double averageScore = 0.0;
};
class Population {
public:
    Population() = default;
    explicit Population(std::vector<Player> players);

    static Population createInitial(int populationSize, std::mt19937& rng);

    std::vector<Player>& allPlayers();
    const std::vector<Player>& allPlayers() const;
    int size() const;

    void resetScores();
    std::map<StrategyType, StrategyStats> calculateStats() const;
    double averageScore() const;
    Population reproduce(int populationSize, double mutationRate, std::mt19937& rng) const;

private:
    std::vector<Player> players;
};
