#include "Simulation.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

namespace {

int readIntArg(char** argv, int index, int defaultValue) {
    if (argv[index] == nullptr) {
        return defaultValue;
    }
    return std::atoi(argv[index]);
}

double readDoubleArg(char** argv, int index, double defaultValue) {
    if (argv[index] == nullptr) {
        return defaultValue;
    }
    return std::atof(argv[index]);
}

void printUsage(const char* executableName) {
    std::cout
        << "Usage:\n"
        << "  " << executableName << " [population] [generations] [rounds] [mutation_rate] [seed] [output_csv]\n\n"
        << "Example:\n"
        << "  " << executableName << " 100 100 100 0.02 42 results/history.csv\n";
}

}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--help") {
        printUsage(argv[0]);
        return 0;
    }

    SimulationConfig config;

    if (argc > 1) {
        config.populationSize = readIntArg(argv, 1, config.populationSize);
    }
    if (argc > 2) {
        config.generations = readIntArg(argv, 2, config.generations);
    }
    if (argc > 3) {
        config.roundsPerMatch = readIntArg(argv, 3, config.roundsPerMatch);
    }
    if (argc > 4) {
        config.mutationRate = readDoubleArg(argv, 4, config.mutationRate);
    }
    if (argc > 5) {
        config.seed = static_cast<unsigned int>(readIntArg(argv, 5, static_cast<int>(config.seed)));
    }
    if (argc > 6) {
        config.outputCsvPath = argv[6];
    }

    try {
        Simulation simulation(config);
        simulation.run();
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
