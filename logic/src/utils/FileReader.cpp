//
// Created by abdellah on 12/11/25.
//

#include "../../include/utils/FileReader.h"

#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> FileReader::getFileLines(const std::string& filename, const bool make) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        if (!make) {
            throw std::runtime_error("Failed to open map file: " + filename);
        }

        // create empty file
        std::ofstream createFile(filename);
        if (!createFile.is_open()) {
            throw std::runtime_error("Failed to create file: " + filename);
        }
        createFile.close();

        // reopen for reading
        file.open(filename);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    if (lines.empty() && !make) {
        throw std::runtime_error("Map file is empty: " + filename);
    }

    return lines;
}

void FileWriter::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}
