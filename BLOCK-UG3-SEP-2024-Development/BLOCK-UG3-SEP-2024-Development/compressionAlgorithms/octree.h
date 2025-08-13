#ifndef OCTREE_H
#define OCTREE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>


class octree{
public:
    octree(const std::vector<std::vector<std::vector<char> > >& parentBlockData,
                  const std::unordered_map<char, std::string>& tagTable,
                  int headerValues[6], int x_offset, int y_offset, int z_offset);
    void startCompression();
    struct Block {
    char character;
    int x, y, z;
    int width, height, depth;
    };
    void compressOctant(int z, int x, int y, int width, int height, int depth);
    char isUniform(int z, int x, int y, int width, int height, int depth);
    bool tryMerge(Block& a, Block& b);
    void merge(std::vector<Block>& subBlocks);
    void check2x2(int z, int x, int y);
    void check2x2x2(int z, int x, int y);
    //void check3x3(int z, int x, int y);
    //void check3x2(int z, int x, int y);
    //void check2x3(int z, int x, int y);
    void printOutput(int z, int x, int y, int width, int height, int depth);
private:
    std::vector<std::vector<std::vector<char> > > parentBlockData;
    std::unordered_map<char, std::string> tagTable;
    int x_count, y_count, z_count, parent_x, parent_y, parent_z;
    std::vector<std::vector<char> > currentOctant;
    int x_offset, y_offset, z_offset;
};
#endif

