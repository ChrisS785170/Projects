#include "LZ78Compression.h"
#include <string>
#include <vector>
#include <map>

std::vector<std::pair<int, char>> compressSlice(const std::string& text){
    std::vector<std::pair<int, char>> compressedData;
    std::unordered_map<std::string, int> dictionary;
    std::string current_string;
    int dictionaryIndex = 1;

    for(char character : text){
        current_string.push_back(character);

        /*if current_string is not in dictionary then add it */
        if(dictionary.find(current_string) == dictionary.end()){
            std::cout << current_string << std::endl;
            dictionary[current_string] = dictionaryIndex++;
            int currSubstrIndex = dictionary[current_string.substr(0, current_string.size() - 1)];
            compressedData.push_back({currSubstrIndex, character});
            current_string = "";
        }
    }
    if(!current_string.empty()){
        int currSubstrIndex = dictionary[current_string.substr(0, current_string.size() - 1)];
        compressedData.push_back({currSubstrIndex, '\0'});
    }

    return compressedData;
}

//Constructor to assign variables
LZ78Compression::LZ78Compression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                             const std::unordered_map<char, std::string>& tagTable,
                             int headerValues[6])
    //parent_x(headerValues[3]),parent_y(headerValues[4]),parent_z(headerValues[5])
{
    //after many hours, i dont think i can use a map for this or i dont know how to.
    //I need to index into  a map but maps cant be "inidexed into" without adding O(N) time complexity.
    //the only other way might be to use a returned iterator, but then i dont think i can store that in the map itself (which is the second parameter)
    std::vector<std::pair<int, char>> allCompressedData;

    // The following code should be changed to simply take in the slices without first being put into a vector model

    //For every slice header values[2] = z_count
    for (int z=0; z<headerValues[2]; z++){
        std::string sliceText = "";

        //For every row in the slice // header values[1] = y_count
        for(int y = 0; y<headerValues[1]; y++){
            // for every char in that row
            for(int x = 0; x<headerValues[0]; x++){
                sliceText += vectorModel[z][y][x];
            }
        }
        
        std::cout << sliceText << std::endl;

        allCompressedData = compressSlice(sliceText);
    }

    //output
    for(int i = 0; i < allCompressedData.size(); i++){
        std::cout << allCompressedData.at(i).first << " : " << allCompressedData.at(i).second << std::endl;
    }

}


    

