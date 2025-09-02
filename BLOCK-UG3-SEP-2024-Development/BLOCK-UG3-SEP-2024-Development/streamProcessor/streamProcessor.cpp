#include "streamProcessor.h"
//#include "../compressionAlgorithms/noCompression.h"
#include "../compressionAlgorithms/RLECompression.h"
#include "../compressionAlgorithms/quadtree.h"
#include "../compressionAlgorithms/octree.h"
#include "../compressionAlgorithms/RLE3D.h"



// Constructor
streamProcessor::streamProcessor(std::istream& input) : inputStream(input) {}

// Start processing
void streamProcessor::startProcessing() {
    std::cout << "Input Algorithm Name: " << std::endl;
    std::string algorithmName;
    std::cin >> algorithmName;
    readHeader();
    readTagTable();
    processData(algorithmName);
}

// Read the header values
void streamProcessor::readHeader() {
    std::string headerLine;
    std::getline(inputStream, headerLine);
    std::istringstream headerStream(headerLine);
    char delimiter;
    headerStream >> headerValues[0] >> delimiter >> headerValues[1] >> delimiter
                 >> headerValues[2] >> delimiter >> headerValues[3] >> delimiter
                 >> headerValues[4] >> delimiter >> headerValues[5];
}

// Read the tag table
void streamProcessor::readTagTable() {
    std::string tagLine;
    while (std::getline(inputStream, tagLine)) {
        if (tagLine.find_first_not_of(" \t\r\n") == std::string::npos) {
            break;
        }
        std::istringstream tagStream(tagLine);
        char delimiter, tempTag;
        std::string tempString;
        tagStream >> tempTag >> delimiter >> tempString;
        tagTable[tempTag] = tempString;
    }
}

// Process data in chunks (per parent block)
void streamProcessor::processData(std::string algorithmName) {
    int x_count = headerValues[0];
    int y_count = headerValues[1];
    int z_count = headerValues[2];
    int parent_x = headerValues[3];
    int parent_y = headerValues[4];
    int parent_z = headerValues[5];

    int num_parent_blocks_x = x_count / parent_x;
    int num_parent_blocks_y = y_count / parent_y;
    int num_parent_blocks_z = z_count / parent_z;

    std::vector<std::vector<std::string>> sliceBuffer(
        parent_z, std::vector<std::string>(y_count));

    for (int pb_z = 0; pb_z < num_parent_blocks_z; ++pb_z) {
        // Read parent_z slices
        for (int dz = 0; dz < parent_z; ++dz) {
            for (int y = 0; y < y_count; ++y) {
                std::string line;
                while (std::getline(inputStream, line)) {
                    if (line.empty()) {
                        continue;
                    } else {
                        break;
                    }
                }
                sliceBuffer[dz][y] = line;
            }
            // Read the empty line that separates slices
            std::string emptyLine;
            std::getline(inputStream, emptyLine);
        }

        // Process parent blocks within these slices
        for (int pb_y = 0; pb_y < num_parent_blocks_y; ++pb_y) {
            for (int pb_x = 0; pb_x < num_parent_blocks_x; ++pb_x) {
                // Extract data for the parent block
                std::vector<std::vector<std::vector<char>>> parentBlockData(
                    parent_z, std::vector<std::vector<char>>(
                        parent_y, std::vector<char>(parent_x)));

                for (int dz = 0; dz < parent_z; ++dz) {
                    for (int dy = 0; dy < parent_y; ++dy) {
                        int global_y = pb_y * parent_y + dy;
                        std::string& line = sliceBuffer[dz][global_y];
                        int start_x = pb_x * parent_x;
                        for (int dx = 0; dx < parent_x; ++dx) {
                            parentBlockData[dz][dy][dx] =
                                line[start_x + dx];
                        }
                    }
                }

                // Process this parent block
                int x_offset = pb_x * parent_x;
                int y_offset = pb_y * parent_y;
                int z_offset = pb_z * parent_z;
                callAlgorithm(algorithmName, parentBlockData,
                              x_offset, y_offset, z_offset);
            }
        }
    }
}

// Updated callAlgorithm function
void streamProcessor::callAlgorithm(
    const std::string& algorithmName,
    const std::vector<std::vector<std::vector<char>>>& parentBlockData,
    int x_offset, int y_offset, int z_offset) {

    if (algorithmName == "octree")
    {
        octree octreeAlgorithm(parentBlockData, tagTable, headerValues, 
                                             x_offset, y_offset, z_offset);
    }else if (algorithmName == "quadtree")
    {
        quadtree quadtreeAlgorithm(parentBlockData, tagTable, headerValues, 
                                             x_offset, y_offset, z_offset);
    }else if (algorithmName == "RLE3D")
    {
        RLE3D rle3DAlgorithm(parentBlockData, tagTable, headerValues, 
                                             x_offset, y_offset, z_offset);
    }

}

// Function to output stored header values (for testing)
void streamProcessor::writeHeader() {
    std::cout << "x_count: " << headerValues[0] << std::endl;
    std::cout << "y_count: " << headerValues[1] << std::endl;
    std::cout << "z_count: " << headerValues[2] << std::endl;
    std::cout << "parent_x: " << headerValues[3] << std::endl;
    std::cout << "parent_y: " << headerValues[4] << std::endl;
    std::cout << "parent_z: " << headerValues[5] << std::endl;
    std::cout << std::endl;
}

// Function to print the stored tag table (for testing)
void streamProcessor::writeTagTable() {
    std::cout << "Tag Table: " << std::endl;
    for (const auto& entry : tagTable) {
        std::cout << entry.first << ", " << entry.second << std::endl;
    }
    std::cout << std::endl;
}

