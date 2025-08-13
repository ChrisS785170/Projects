#ifndef STREAMPROCESSOR_H
#define STREAMPROCESSOR_H

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

class streamProcessor{
public:
    streamProcessor(std::istream& input);
    void startProcessing();
private:
    void readHeader();
    void readTagTable();
    void processData(std::string algorithmName);

    void writeHeader(); // for testing only
    void writeTagTable(); // for testing only
    // void writeSlices(); // No longer needed

    // Updated function signature with additional parameters
    void callAlgorithm(const std::string& algorithmName,
                       const std::vector<std::vector<std::vector<char>>>& parentBlockData,
                       int x_offset, int y_offset, int z_offset);

    std::istream& inputStream;
    int headerValues[6]; // x_count, y_count, z_count, parent_x, parent_y, parent_z
    std::unordered_map<char, std::string> tagTable;
    // std::vector<std::vector<std::vector<char>>> vectorModel; // Remove this line
};
#endif // STREAMPROCESSOR_H