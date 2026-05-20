#pragma once

#include "Game.hpp"
#include "Population.hpp"

#include <filesystem>
#include <random>
#include <string>

struct SimulationConfig {
    int populationSize = 100;
    int generations = 100;
    int roundsPerMatch = 100;
    double mutationRate = 0.02;
    unsigned int seed = 42;
    PayoffMatrix payoffMatrix{};
    std::filesystem::path outputCsvPath = "results/history.csv";
};

class Simulation {
public:
    explicit Simulation(SimulationConfig config);
    void run();

private:
    SimulationConfig config;
    std::mt19937 rng;
};
