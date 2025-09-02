#include "noCompression.h"

//Constructor to assign variables
noCompression::noCompression(const std::vector<std::vector<std::vector<char> > >& vectorModel,
                             const std::unordered_map<char, std::string>& tagTable,
                             int headerValues[6])
    : vectorModel(vectorModel), tagTable(tagTable), 
    x_count(headerValues[0]), y_count(headerValues[1]), z_count(headerValues[2]),
    parent_x(headerValues[3]),parent_y(headerValues[4]),parent_z(headerValues[5]){
        startCompression();
    }

//This is an example for how the algorithm could work on each individual element of the vector
void noCompression::startCompression(){
    int ex;
    //For every slice
    for (int z=0; z<z_count; z++){
        //For every line in the slice
        for(int y = 0; y<y_count; y++){
            //For every character in each line
            for(int x = 0; x<x_count; x++){
                //vectorModel[z][y][x] will access all characters
                ex++;
            }
        }
    }
    startOutput();
}

//Function to ouput the results in the correct format, will be different based on different compression algorithms
void noCompression::startOutput(){
std::string tagVal;
    for (int z=0; z<z_count; z++){
        for(int y = 0; y<y_count; y++){
            for(int x = 0; x<x_count; x++){
                char currentChar = vectorModel[z][y][x];
                std::unordered_map<char, std::string>::iterator it = tagTable.find(currentChar);
                if(it != tagTable.end()){
                    tagVal = it->second;
                }else{
                    tagVal = "";
                }
                std::cout<<x<<','<<y<<','<<z<<','<<"1,1,1,"<<tagVal<<std::endl;
            }
        }
    }

}


    

