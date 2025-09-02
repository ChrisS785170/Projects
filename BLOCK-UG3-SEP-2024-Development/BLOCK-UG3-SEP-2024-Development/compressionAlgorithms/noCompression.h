#ifndef NOCOMPRESSION_H
#define NOCOMPRESSION_H

#include <iostream>
#include <unordered_map>
#include <vector>

class noCompression{
public:
    noCompression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                  const std::unordered_map<char, std::string>& tagTable,
                  int headerValues[6]);
    void startCompression();
    void startOutput();
private:
    std::vector<std::vector<std::vector<char> > > vectorModel;
    std::unordered_map<char, std::string> tagTable;
    int x_count, y_count, z_count, parent_x, parent_y, parent_z;
};
#endif