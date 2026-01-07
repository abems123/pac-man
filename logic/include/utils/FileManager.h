//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_FILEREADER_H
#define PACMANPROJECT_FILEREADER_H
#include <string>
#include <vector>

namespace logic {

/**
 * @brief Utility for reading text files.
 */
struct FileReader {
    /**
     * @brief Reads a text file into a list of lines.
     * @param filename Path to the file.
     * @param make If true, create the file if it does not exist (implementation-dependent).
     * @return Lines of the file in order.
     */
    [[nodiscard]] static std::vector<std::string> getFileLines(const std::string& filename, bool make = false);
};

/**
 * @brief Utility for writing text files.
 */
struct FileWriter {
    /**
     * @brief Writes content to a text file.
     * @param filename Path to the file.
     * @param content Full content to write.
     * @return Nothing.
     */
    static void writeFile(const std::string& filename, const std::string& content);
};
} // namespace logic
#endif // PACMANPROJECT_FILEREADER_H
