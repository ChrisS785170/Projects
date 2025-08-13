#ifndef RLECOMPRESSION_H
#define RLECOMPRESSION_H

#include <iostream>
#include <unordered_map>
#include <vector>

class RLECompression {
    public:
        RLECompression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                  const std::unordered_map<char, std::string>& tagTable,
                  int headerValues[6]);
        void startCompression();
        void startOutput();
    private:
        std::vector<std::vector<std::vector<char> > > vectorModel;
        std::unordered_map<char, std::string> tagTable;
        int x_count, y_count, z_count, parent_x, parent_y, parent_z;

        struct Block {
            int x, y, z;
            int length;
            char tag;
        };

        std::vector<Block> compressedBlocks;
};
#endif