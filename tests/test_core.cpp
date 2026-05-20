#include "CsvWriter.hpp"
#include "Game.hpp"
#include "PayoffMatrix.hpp"
#include "Strategy.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <string>

void testPayoffMatrix() {
    PayoffMatrix matrix;

    assert(matrix.scoreFor(Action::Cooperate, Action::Cooperate) == 3);
    assert(matrix.scoreFor(Action::Defect, Action::Cooperate) == 5);
    assert(matrix.scoreFor(Action::Cooperate, Action::Defect) == 0);
    assert(matrix.scoreFor(Action::Defect, Action::Defect) == 1);
}

void testAlwaysStrategies() {
    std::mt19937 rng(42);
    AlwaysCooperateStrategy cooperate;
    AlwaysDefectStrategy defect;
    std::vector<Action> empty;

    assert(cooperate.decideAction(0, empty, empty, rng) == Action::Cooperate);
    assert(defect.decideAction(0, empty, empty, rng) == Action::Defect);
}

void testTitForTat() {
    std::mt19937 rng(42);
    TitForTatStrategy titForTat;

    std::vector<Action> myHistory;
    std::vector<Action> opponentHistory;

    assert(titForTat.decideAction(0, myHistory, opponentHistory, rng) == Action::Cooperate);

    opponentHistory.push_back(Action::Defect);
    assert(titForTat.decideAction(1, myHistory, opponentHistory, rng) == Action::Defect);

    opponentHistory.push_back(Action::Cooperate);
    assert(titForTat.decideAction(2, myHistory, opponentHistory, rng) == Action::Cooperate);
}

void testCsvWriter() {
    const std::filesystem::path outputPath = "results/test_history.csv";
    {
        CsvWriter writer(outputPath);
        writer.writeHeader();

        std::map<StrategyType, StrategyStats> stats;
        for (StrategyType type : allStrategyTypes()) {
            stats[type] = StrategyStats{};
        }
        stats[StrategyType::AlwaysCooperate].count = 2;
        stats[StrategyType::AlwaysDefect].count = 3;

        writer.writeGeneration(0, stats, 12.5);
    }

    std::ifstream file(outputPath);
    assert(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string content = buffer.str();

    assert(content.find("generation,AlwaysCooperate,AlwaysDefect,TitForTat,GrimTrigger,RandomStrategy,Pavlov,avg_score") != std::string::npos);
    assert(content.find("0,2,3,0,0,0,0,12.5000") != std::string::npos);

    file.close();
    std::filesystem::remove(outputPath);
}

int main() {
    testPayoffMatrix();
    testAlwaysStrategies();
    testTitForTat();
    testCsvWriter();
    return 0;
}
