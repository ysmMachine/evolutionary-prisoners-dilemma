#include "Strategy.hpp"

#include "PayoffMatrix.hpp"

#include <algorithm>

std::vector<StrategyType> allStrategyTypes() {
    return {
        StrategyType::AlwaysCooperate,
        StrategyType::AlwaysDefect,
        StrategyType::TitForTat,
        StrategyType::GrimTrigger,
        StrategyType::RandomStrategy,
        StrategyType::Pavlov
    };
}

std::string strategyTypeToString(StrategyType type) {
    switch (type) {
    case StrategyType::AlwaysCooperate:
        return "AlwaysCooperate";
    case StrategyType::AlwaysDefect:
        return "AlwaysDefect";
    case StrategyType::TitForTat:
        return "TitForTat";
    case StrategyType::GrimTrigger:
        return "GrimTrigger";
    case StrategyType::RandomStrategy:
        return "RandomStrategy";
    case StrategyType::Pavlov:
        return "Pavlov";
    }
    return "Unknown";
}

void Strategy::reset() {
}

StrategyType AlwaysCooperateStrategy::type() const {
    return StrategyType::AlwaysCooperate;
}

std::string AlwaysCooperateStrategy::name() const {
    return strategyTypeToString(type());
}

Action AlwaysCooperateStrategy::decideAction(int, const std::vector<Action>&, const std::vector<Action>&, std::mt19937&) {
    return Action::Cooperate;
}

StrategyType AlwaysDefectStrategy::type() const {
    return StrategyType::AlwaysDefect;
}

std::string AlwaysDefectStrategy::name() const {
    return strategyTypeToString(type());
}

Action AlwaysDefectStrategy::decideAction(int, const std::vector<Action>&, const std::vector<Action>&, std::mt19937&) {
    return Action::Defect;
}

StrategyType TitForTatStrategy::type() const {
    return StrategyType::TitForTat;
}

std::string TitForTatStrategy::name() const {
    return strategyTypeToString(type());
}

Action TitForTatStrategy::decideAction(int roundIndex, const std::vector<Action>&, const std::vector<Action>& opponentHistory, std::mt19937&) {
    if (roundIndex == 0 || opponentHistory.empty()) {
        return Action::Cooperate;
    }
    return opponentHistory.back();
}

StrategyType GrimTriggerStrategy::type() const {
    return StrategyType::GrimTrigger;
}

std::string GrimTriggerStrategy::name() const {
    return strategyTypeToString(type());
}

void GrimTriggerStrategy::reset() {
    opponentDefected = false;
}

Action GrimTriggerStrategy::decideAction(int, const std::vector<Action>&, const std::vector<Action>& opponentHistory, std::mt19937&) {
    if (std::find(opponentHistory.begin(), opponentHistory.end(), Action::Defect) != opponentHistory.end()) {
        opponentDefected = true;
    }
    return opponentDefected ? Action::Defect : Action::Cooperate;
}

RandomStrategy::RandomStrategy(double cooperateProbability)
    : cooperateProbability(cooperateProbability) {
}

StrategyType RandomStrategy::type() const {
    return StrategyType::RandomStrategy;
}

std::string RandomStrategy::name() const {
    return strategyTypeToString(type());
}

Action RandomStrategy::decideAction(int, const std::vector<Action>&, const std::vector<Action>&, std::mt19937& rng) {
    std::bernoulli_distribution cooperate(cooperateProbability);
    return cooperate(rng) ? Action::Cooperate : Action::Defect;
}

StrategyType PavlovStrategy::type() const {
    return StrategyType::Pavlov;
}

std::string PavlovStrategy::name() const {
    return strategyTypeToString(type());
}

Action PavlovStrategy::decideAction(int roundIndex, const std::vector<Action>& myHistory, const std::vector<Action>& opponentHistory, std::mt19937&) {
    if (roundIndex == 0 || myHistory.empty() || opponentHistory.empty()) {
        return Action::Cooperate;
    }

    const Action myLast = myHistory.back();
    const Action opponentLast = opponentHistory.back();

    const bool wasGoodOutcome =
        (myLast == Action::Cooperate && opponentLast == Action::Cooperate) ||
        (myLast == Action::Defect && opponentLast == Action::Cooperate);

    return wasGoodOutcome ? myLast : oppositeAction(myLast);
}

std::unique_ptr<Strategy> createStrategy(StrategyType type) {
    switch (type) {
    case StrategyType::AlwaysCooperate:
        return std::make_unique<AlwaysCooperateStrategy>();
    case StrategyType::AlwaysDefect:
        return std::make_unique<AlwaysDefectStrategy>();
    case StrategyType::TitForTat:
        return std::make_unique<TitForTatStrategy>();
    case StrategyType::GrimTrigger:
        return std::make_unique<GrimTriggerStrategy>();
    case StrategyType::RandomStrategy:
        return std::make_unique<RandomStrategy>();
    case StrategyType::Pavlov:
        return std::make_unique<PavlovStrategy>();
    }
    return std::make_unique<TitForTatStrategy>();
}
