#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>


class quadtree{
public:
    quadtree(const std::vector<std::vector<std::vector<char> > >& parentBlockData,
                  const std::unordered_map<char, std::string>& tagTable,
                  int headerValues[6], int x_offset, int y_offset, int z_offset);
    void startCompression();
    void compressQuadrant(int x, int y, int z, int width, int height);
    bool isUniform(int x, int y, int z, int width, int height);
    void check2x2(int x, int y, int z);
    void printOutput(int x, int y, int z, int width, int height);
private:
    std::vector<std::vector<std::vector<char> > > parentBlockData;
    std::unordered_map<char, std::string> tagTable;
    int x_count, y_count, z_count, parent_x, parent_y, parent_z;
    int x_offset, y_offset, z_offset;
    std::vector<std::vector<char> > currentQuadrant;
};
#endif

