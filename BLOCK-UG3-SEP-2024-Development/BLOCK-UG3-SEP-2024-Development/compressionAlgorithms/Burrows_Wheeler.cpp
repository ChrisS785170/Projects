#include "Burrows_Wheeler.h"
#include <string>
#include <cstring>
#include <vector>
#include <map>

// the alogorithm relating to Burrows-Wheeler transform(BWT), an algorithm which takes in the input stream and transforms it into an easy to compress string. BWT is therefore 
// a method that improves effiency of text compression algorithms for little computational power.    

//currently O(n^2) can be better but will always be slower then just using the input naturally 
    
Burrows_Wheeler::Burrows_Wheeler(const std::vector<std::vector<std::vector<char> > >& vectorModel, const std::unordered_map<char, std::string>& tagTable, int headerValues[6]) {

    // set all parameters in easier to read format
    int x_count = headerValues[0];
    int y_count = headerValues[1];
    int z_count = headerValues[2];

    int parent_x = headerValues[3];
    int parent_y = headerValues[4];
    int parent_z = headerValues[5];

    


    std::vector<std::vector<std::vector<char> > > input;
    std::vector<std::vector<std::vector<char> > > sorted_input;
    input = vectorModel; // so that can change 
    std::vector<std::vector<char>>BWT;  

    // maybe do +1 
    
    // loop through all slices 
    for (int z = 0; z<z_count; z++){
        // loop through all rows 
        for (int y = 0; y<y_count; y++){
            input[z][y].push_back('{'); // adding the needed end of line character, used to decompress for output reasons, we use this character as its the next character after z in ASCII table


            // make all permutations of the row and put them in the array 
            for(int i = 0; i<x_count+1; i++){ // the x_count + 1 to account for the {
                int c = 0; // counter varible
                
                for(int x = 0; x<x_count+1; x++){
                    if(x+i <x_count+1){
                        BWT[i][x] = input[z][y][x+i]; 
                    }else{ // for if x+i exceeds array bounds start getting characters from the begining of the array
                        BWT[i][x] = input[z][y][c];
                        c=c+1;
                    }


                }

            }

            // Sorting all perutations into alphabetical order  
            int n = 0;
            char place[2] = {'$','$'};



            // making the vector into an array so that strcmp can be used
            char** arr = new char*[x_count+1];
            char** sorted = new char*[x_count+1];
            for (int i = 0; i < x_count+1; ++i) {
                arr[i] = new char[x_count+1];
            }

            // Copy elements from the 2D vector to the 2D array
            for (int i = 0; i < x_count+1; ++i) {
                for (int j = 0; j < x_count+1; ++j) {
                    arr[i][j] = BWT[i][j];
                }
            }
            // now that its and array we can use strcmp to create a sorted array
            for(int j = 0; j<x_count+1; j++){
                n = 0;
                for(int i = 0; i<x_count+1; i++){
                    if (strcmp(arr[n],arr[i]) < 0 ){ // basically if arr[i] is alphabetically first then make it arr[n]
                        n = i;

                    }

                }
                sorted[j] = arr[n]; // add it to the next position in sorted 
                arr[n] = place; // make arr[n] $$ which is the higher then any ascii value we will encounter this might need to be the same length as the array but idk yet
            }
            
            for (int i = 0; i< x_count+1; i++){
                sorted_input[z][y][i] = sorted[i][x_count+1]; // take the last character in order of each sorted permutation and create new sorted input in accords to burrows wheeler transformation

            }

    }
        
    }




};
