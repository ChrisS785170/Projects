#include "streamProcessor/streamProcessor.h"
#include <fstream>

int main(int agrc, char* argv[]){

    //streamProcessor processor(std::cin);
    //processor.startProcessing();
    
    std::string fileLocation;
    std::cout << "Input file location: " << std::endl;

    std::cin >> fileLocation;
    
    std::fstream inputFile(fileLocation);
    if (!inputFile.is_open()){
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }
    
    streamProcessor processor(inputFile);
    processor.startProcessing();
    inputFile.close();
    
    return 0;
}