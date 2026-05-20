#pragma once

#include "Action.hpp"

#include <memory>
#include <random>
#include <string>
#include <vector>
enum class StrategyType {
    AlwaysCooperate,
    AlwaysDefect,
    TitForTat,
    GrimTrigger,
    RandomStrategy,
    Pavlov
};

std::vector<StrategyType> allStrategyTypes();
std::string strategyTypeToString(StrategyType type);
class Strategy {
public:
    virtual ~Strategy() = default;

    virtual StrategyType type() const = 0;
    virtual std::string name() const = 0;
    virtual void reset();
    virtual Action decideAction(
        int roundIndex,
        const std::vector<Action>& myHistory,
        const std::vector<Action>& opponentHistory,
        std::mt19937& rng
    ) = 0;
};

class AlwaysCooperateStrategy : public Strategy {
public:
    StrategyType type() const override;
    std::string name() const override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;
};

class AlwaysDefectStrategy : public Strategy {
public:
    StrategyType type() const override;
    std::string name() const override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;
};

class TitForTatStrategy : public Strategy {
public:
    StrategyType type() const override;
    std::string name() const override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;
};

class GrimTriggerStrategy : public Strategy {
public:
    StrategyType type() const override;
    std::string name() const override;
    void reset() override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;

private:
    bool opponentDefected = false;
};

class RandomStrategy : public Strategy {
public:
    explicit RandomStrategy(double cooperateProbability = 0.5);

    StrategyType type() const override;
    std::string name() const override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;

private:
    double cooperateProbability;
};

class PavlovStrategy : public Strategy {
public:
    StrategyType type() const override;
    std::string name() const override;
    Action decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937& rng) override;
};

std::unique_ptr<Strategy> createStrategy(StrategyType type);
