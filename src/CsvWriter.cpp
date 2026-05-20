#include "CsvWriter.hpp"

#include <iomanip>
#include <stdexcept>

CsvWriter::CsvWriter(const std::filesystem::path& filePath) {
    const std::filesystem::path parent = filePath.parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    file.open(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("CSV 파일을 열 수 없습니다: " + filePath.string());
    }
}

void CsvWriter::writeHeader() {
    file << "generation";
    for (StrategyType type : allStrategyTypes()) {
        file << "," << strategyTypeToString(type);
    }
    file << ",avg_score\n";
}

void CsvWriter::writeGeneration(int generation, const std::map<StrategyType, StrategyStats>& stats, double averageScore) {
    file << generation;

    for (StrategyType type : allStrategyTypes()) {
        const auto found = stats.find(type);
        const int count = found == stats.end() ? 0 : found->second.count;
        file << "," << count;
    }

    file << "," << std::fixed << std::setprecision(4) << averageScore << "\n";
}
