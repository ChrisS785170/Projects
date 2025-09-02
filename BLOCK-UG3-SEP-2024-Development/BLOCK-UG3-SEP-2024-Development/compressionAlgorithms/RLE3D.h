#ifndef RLE3D_H
#define RLE3D_H

#include <iostream>
#include <unordered_map>
#include <vector>

class RLE3D {
    public:
        RLE3D(const std::vector<std::vector<std::vector<char>>>& parentBlockData,
          const std::unordered_map<char, std::string>& tagTable,
          int headerValues[6], int x_offset, int y_offset, int z_offset);
        void startCompression();
        void startOutput();
    private:
        const std::vector<std::vector<std::vector<char>>>& parentBlockData;
        const std::unordered_map<char, std::string>& tagTable;
        int x_offset;
        int y_offset;
        int z_offset;
        int x_count, y_count, z_count, parent_x, parent_y, parent_z;

        struct Block {
            int x, y, z;
            int x_size, y_size, z_size;
            char tag;
        };

        std::vector<Block> compressedBlocks;
};
#endif