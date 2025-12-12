//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_FILEREADER_H
#define PACMANPROJECT_FILEREADER_H
#include <string>
#include <vector>


struct FileReader {
    static std::vector<std::string> getFileLines(std::string filename);

};


#endif //PACMANPROJECT_FILEREADER_H
