#include "Simulation.hpp"

#include "CsvWriter.hpp"

#include <iostream>
#include <utility>

Simulation::Simulation(SimulationConfig config)
    : config(std::move(config)),
      rng(this->config.seed) {
}

void Simulation::run() {
    Population population = Population::createInitial(config.populationSize, rng);
    Game game(config.payoffMatrix, config.roundsPerMatch);
    CsvWriter writer(config.outputCsvPath);
    writer.writeHeader();

    for (int generation = 0; generation <= config.generations; ++generation) {
        population.resetScores();

        std::vector<Player>& players = population.allPlayers();

        for (int i = 0; i < population.size(); ++i) {
            for (int j = i + 1; j < population.size(); ++j) {
                game.play(players[i], players[j], rng);
            }
        }

        const auto stats = population.calculateStats();

        const double normalizer = static_cast<double>((population.size() - 1) * config.roundsPerMatch);
        const double average = normalizer > 0.0 ? population.averageScore() / normalizer : 0.0;
        writer.writeGeneration(generation, stats, average);

        if (generation % 10 == 0 || generation == config.generations) {
            std::cout << "Generation " << generation << " saved. Average score: " << average << "\n";
        }

        if (generation < config.generations) {
            population = population.reproduce(config.populationSize, config.mutationRate, rng);
        }
    }

    std::cout << "Simulation completed. CSV: " << config.outputCsvPath << "\n";
}
