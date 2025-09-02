#include "streamProcessor.h"
#include "../compressionAlgorithms/noCompression.h"
#include "../compressionAlgorithms/RLECompression.h"
#include "../compressionAlgorithms/quadtree.h"

// Constructor for the streamProcessor class, requires an input file.
void streamProcessor(std::istream& input){
    std::unordered_map<char, std::string> tagTable;
    std::vector<std::vector<std::vector<char> > > vectorModel;
    int headerValues[6]; // Header Values stores the following values in this order: x_count, y_count, z_count, parent_x, parent_y, parent_z


// Start processing the input data. it will read the  header, TagTable and Slices.
// It will then call the output functions to display the stored results.

    // Read the header of the file. This will strip the first line from the input

        // Initialise the string to store the header values in
        std::string headerLine;
        // Get the next line from the input. This will be the first line in the test text file which should be the header
        std::getline(input, headerLine);
        // Create an istringstream with the headerline read from the test file. 
        std::istringstream headerStream(headerLine);
        // delimiter is used as a temporary storage for the commas in the test file header line. 
        char delimiter;
        // Strip out all the required values from the istringstream and store them in the header values array.
        // Header values is in the following form:
        // {x_count, y_count, z_count, parent_x, parent_y, parent_z}
        headerStream >> headerValues[0] >> delimiter>> headerValues[1] >> delimiter>> headerValues[2] >>delimiter>> 
        headerValues[3] >>delimiter>> headerValues[4] >> delimiter>>  headerValues[5];

/*
    // Call the function to read the test files tag table. This will strip all tags from the input
    readTagTable();
    // Call the function to read the test files slices. This will read every slice and store it in a 
    // 3 dimensional vector. This vector is then used for the compression. 
    readSlices();

    // Prompt the tester to input the chosen algorithm 
    std::cout << "Input algorithm name: " << std::endl;

    std::string algorithmName;
    // Read in the chosen algorithm
    std::cin >> algorithmName;
    //Call the callAlgorithm function which will call the correct function which is stored in the compressionAlgorithms branch.
    callAlgorithm(algorithmName);  
*/
//}


    // Function to read the tagTable and store in an unordered map
    //void streamProcessor::readTagTable(){
        // Empty string to store the current line in
        std::string tagLine;
        // Loop until a blank line is encountered
        while(std::getline(input, tagLine)){
            // If a blank line
            if(tagLine.find_first_not_of(" \t\r\n") == std::string::npos){
                break;
            }
            // Create a string stream from this string
            std::istringstream tagStream(tagLine);
            // delimiter is a variable to store the commas, tempTag will store the character (key)
            char delimiter, tempTag;
            // tempString will store the label
            std::string tempString;
            // store tag in tempTag and label in tempString
            tagStream >> tempTag >> delimiter >> tempString;
            // Store in the tagTable (map)
            tagTable[tempTag] = tempString;
        }
    //}

    // Function to read each slice and store the values in a 3D vector
    //void streamProcessor::readSlices(){
        // Initialise the empty 3D vector to be the correct size based on the x, y and z count
        // vectorModel stores z slices which are each y vectors with each vector being size x
        vectorModel = std::vector<std::vector<std::vector<char> > >(
            headerValues[2], std::vector<std::vector<char> >(
                headerValues[1], std::vector<char>(headerValues[0])
            )
        );
        // Initialise an empty string to read each line from the current slice
        std::string sliceLine;
        // initialise z to be 0
        int z = 0;
        // Until all slices have read
            while(z<headerValues[2]){
                // Initialise y to be 0
                int y = 0;
                // Until all of the have been read in the current slice
                while (y<headerValues[1] && std::getline (input, sliceLine)){
                    // THIS COULD BE REMOVED - REILLY TO REVIEW WITH SHEA
                    if (sliceLine.size()< headerValues[0]){
                        // 
                        continue;
                    }
                    // Loop through all characters in the current line
                    for (int x =0; x<headerValues[0]; x++){
                        // Assign the correct charactet from the current sliceLine
                        vectorModel[z][y][x] = sliceLine[x];
                    }
                    y++;
                }
                z++;
        }
    //}
/*          FIX THIS WITH ANOTHER FUNCTION CALL THAT TAKES A FUNCTION CALL + THE DATA: E.G callAlg(data,compression_func); 
            THAT WAY THERES NO STRING COMPARISON WHICH IS SLOW
    //void streamProcessor::callAlgorithm(std::string algorithmName){
        if(algorithmName == "noCompression"){
            noCompression noCompressionAlgorithm(vectorModel, tagTable, headerValues);
        }
        else if (algorithmName == "RLECompression")
        {
            RLECompression rleCompressionAlgorithm(vectorModel, tagTable, headerValues);
        }else if (algorithmName == "quadtree")
        {
            quadtree quadtreeAlgorithm(vectorModel, tagTable, headerValues);
        }
        
    //}
*/
    // Function to output stored header values
    //void streamProcessor::writeHeader(){
        std::cout<<"x_count: "<<headerValues[0]<<std::endl;
        std::cout<<"y_count: "<<headerValues[1]<<std::endl;
        std::cout<<"z_count: "<<headerValues[2]<<std::endl;
        std::cout<<"parent_x: "<<headerValues[3]<<std::endl;
        std::cout<<"parent_y: "<<headerValues[4]<<std::endl;
        std::cout<<"parent_z: "<<headerValues[5]<<std::endl;

        std::cout << std::endl;
    //}

    // Function to print the stored tag table
    //void streamProcessor::writeTagTable(){
        // Create iterator for printing of tagTable
        std::unordered_map<char, std::string>::iterator it = tagTable.begin();

        std::cout << "Tag Table: " << std::endl;
        // Iterate through entire tagTable and print each label, tag pair
        while(it != tagTable.end()){
            // get label
            char label = it->first;
            // get tag
            std::string tag = it->second;
            // print label then tag (example: "o, sea")
            std::cout << label << ", " << tag << std::endl;
            // Go to next map entry
            it++;
        }

        std::cout << std::endl;
    //}

    // Function to print each slice
    //void streamProcessor::writeSlices(){
        for(int z = 0; z < headerValues[2]; z++){
            std::cout << "Slice: " << z << std::endl;
            for(int y = 0; y < headerValues[1]; y++){
                for(int x = 0; x < headerValues[0] ; x++){
                    std::cout << vectorModel[z][y][x];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    //}
}