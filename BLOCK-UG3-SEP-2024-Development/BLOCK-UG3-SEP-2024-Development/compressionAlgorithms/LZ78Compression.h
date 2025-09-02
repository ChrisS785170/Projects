#ifndef LZ78COMPRESSION_H
#define LZ78COMPRESSION_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>

class LZ78Compression{
    public:
        LZ78Compression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                        const std::unordered_map<char, std::string>& tagTable,
                        int headerValues[6]);
    };

#endif