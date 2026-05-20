#pragma once

#include "Strategy.hpp"

#include <memory>
class Player {
public:
    explicit Player(std::unique_ptr<Strategy> strategy);
    Strategy& getStrategy();
    const Strategy& getStrategy() const;

    StrategyType strategyType() const;

    void resetScore();
    void addScore(double value);
    double score() const;

private:
    std::unique_ptr<Strategy> strategy;
    double totalScore = 0.0;
};
