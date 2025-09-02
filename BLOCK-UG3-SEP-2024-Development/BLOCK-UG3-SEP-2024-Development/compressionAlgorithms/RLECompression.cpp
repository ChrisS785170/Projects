#include "RLECompression.h"

//Constructor to assign variables
RLECompression::RLECompression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                             const std::unordered_map<char, std::string>& tagTable,
                             int headerValues[6])
    : vectorModel(vectorModel), tagTable(tagTable), 
    x_count(headerValues[0]), y_count(headerValues[1]), z_count(headerValues[2]),
    parent_x(headerValues[3]),parent_y(headerValues[4]),parent_z(headerValues[5]){
        startCompression();
    }

// This function implements the Run Length Encoding (RLE) compression algorithm
void RLECompression::startCompression() {
    // Loop through each slice (z-axis)
    for (int z = 0; z < z_count; z++) {
        // Loop through each (y-axis)
        for (int y = 0; y < y_count; y++) {
            int x = 0;
            while (x < x_count) {
                char currentTag = vectorModel[z][y][x];
                int runLength = 1;

                // Calculate run length of the current block (consecutive identical blocks)
                while (x + runLength < x_count && vectorModel[z][y][x + runLength] == currentTag) {
                    if ((x + runLength) % parent_x == 0) {
                        break;
                    }
                    runLength++;
                }

                // Store the compressed block information
                compressedBlocks.push_back({x, y, z, runLength, currentTag});

                // Move the next block
                x += runLength;
            }
        }
    }
    startOutput();
}

// Function to output the compressed blocks in the required format.
void RLECompression::startOutput(){
    for (const Block& block : compressedBlocks) {
        std::unordered_map<char, std::string>::iterator it = tagTable.find(block.tag);
        std::string tagVal = (it != tagTable.end()) ? it->second : "";

        std::cout << block.x << ',' << block.y << ',' << block.z << ','
                  << block.length << ",1,1," << tagVal << std::endl;
    }
}
