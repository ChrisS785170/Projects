#ifndef BURROWS_WHEELER_H
#define BURROWS_WHEELER_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>

class Burrows_Wheeler{
    public:
        Burrows_Wheeler(const std::vector<std::vector<std::vector<char> > >& vectorModel, const std::unordered_map<char, std::string>& tagTable, int headerValues[6]);
    };

#endif