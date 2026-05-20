#pragma once

#include "Population.hpp"

#include <filesystem>
#include <fstream>
#include <string>

class CsvWriter {
public:
    explicit CsvWriter(const std::filesystem::path& filePath);

    void writeHeader();
    void writeGeneration(int generation, const std::map<StrategyType, StrategyStats>& stats, double averageScore);

private:
    std::ofstream file;
};
