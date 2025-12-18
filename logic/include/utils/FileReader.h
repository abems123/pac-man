//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_FILEREADER_H
#define PACMANPROJECT_FILEREADER_H
#include <string>
#include <vector>

class FileManager {
};

struct FileReader : public FileManager {
    [[nodiscard]] static std::vector<std::string> getFileLines(const std::string &filename, bool make=false);
};

struct FileWriter : public FileManager {
    static void writeFile(const std::string &filename, const std::string &content);
};


#endif //PACMANPROJECT_FILEREADER_H
