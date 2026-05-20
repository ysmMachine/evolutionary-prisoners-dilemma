#include "Population.hpp"

#include <algorithm>
#include <numeric>
#include <utility>

Population::Population(std::vector<Player> players)
    : players(std::move(players)) {
}

Population Population::createInitial(int populationSize, std::mt19937& rng) {
    std::vector<Player> initialPlayers;
    initialPlayers.reserve(populationSize);

    const std::vector<StrategyType> types = allStrategyTypes();
    std::uniform_int_distribution<int> pickStrategy(0, static_cast<int>(types.size()) - 1);

    for (int i = 0; i < populationSize; ++i) {
        initialPlayers.emplace_back(createStrategy(types[pickStrategy(rng)]));
    }

    return Population(std::move(initialPlayers));
}

std::vector<Player>& Population::allPlayers() {
    return players;
}

const std::vector<Player>& Population::allPlayers() const {
    return players;
}

int Population::size() const {
    return static_cast<int>(players.size());
}

void Population::resetScores() {
    for (Player& player : players) {
        player.resetScore();
    }
}

std::map<StrategyType, StrategyStats> Population::calculateStats() const {
    std::map<StrategyType, StrategyStats> stats;

    for (StrategyType type : allStrategyTypes()) {
        stats[type] = StrategyStats{};
    }

    for (const Player& player : players) {
        StrategyStats& strategyStats = stats[player.strategyType()];
        strategyStats.count += 1;
        strategyStats.totalScore += player.score();
    }

    for (auto& [type, strategyStats] : stats) {
        if (strategyStats.count > 0) {
            strategyStats.averageScore = strategyStats.totalScore / strategyStats.count;
        }
    }

    return stats;
}

double Population::averageScore() const {
    if (players.empty()) {
        return 0.0;
    }

    const double total = std::accumulate(
        players.begin(),
        players.end(),
        0.0,
        [](double sum, const Player& player) {
            return sum + player.score();
        }
    );

    return total / players.size();
}

Population Population::reproduce(int populationSize, double mutationRate, std::mt19937& rng) const {
    std::vector<Player> nextPlayers;
    nextPlayers.reserve(populationSize);

    std::vector<double> weights;
    weights.reserve(players.size());

    for (const Player& player : players) {
        weights.push_back(std::max(0.0, player.score()) + 1.0);
    }

    std::discrete_distribution<int> pickParent(weights.begin(), weights.end());
    std::bernoulli_distribution mutate(mutationRate);

    const std::vector<StrategyType> types = allStrategyTypes();
    std::uniform_int_distribution<int> pickMutation(0, static_cast<int>(types.size()) - 1);

    for (int i = 0; i < populationSize; ++i) {
        const Player& parent = players[pickParent(rng)];
        StrategyType childType = parent.strategyType();

        if (mutate(rng)) {
            childType = types[pickMutation(rng)];
        }

        nextPlayers.emplace_back(createStrategy(childType));
    }

    return Population(std::move(nextPlayers));
}
