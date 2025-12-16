//
// Created by abdellah on 12/11/25.
//

#include "../../include/utils/FileReader.h"

#include <fstream>
#include <sstream>
#include <vector>


std::vector<std::string> FileReader::getFileLines(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

void FileWriter::writeFile(const std::string &filename, const std::string &content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}
