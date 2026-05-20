#include "Player.hpp"

#include <utility>

Player::Player(std::unique_ptr<Strategy> strategy)
    : strategy(std::move(strategy)) {
}

Strategy& Player::getStrategy() {
    return *strategy;
}

const Strategy& Player::getStrategy() const {
    return *strategy;
}

StrategyType Player::strategyType() const {
    return strategy->type();
}

void Player::resetScore() {
    totalScore = 0.0;
}

void Player::addScore(double value) {
    totalScore += value;
}

double Player::score() const {
    return totalScore;
}
