#include "quadtree.h"
// Quadtree compession algorithms, recursively divides slices into 4 smaller quadtrants and checks
// if uniform. 
// testFiles/test_data_file_32768_4x4x4.csv
// testFiles/the_fast_one_376000_2x2x2.csv
// x86_64-w64-mingw32-g++ -static -o unitek-quadtree.exe main.cpp streamProcessor/streamProcessor.cpp 
// compressionAlgorithms/quadtree.cpp compressionAlgorithms/noCompression.cpp compressionAlgorithms/RLECompression.cpp

// Constructor to assign variables
quadtree::quadtree(const std::vector<std::vector<std::vector<char> > >& parentBlockData,
                             const std::unordered_map<char, std::string>& tagTable,
                             int headerValues[6], int x_offset, int y_offset, int z_offset)
    : parentBlockData(parentBlockData), tagTable(tagTable), 
    x_offset(x_offset), y_offset(y_offset), z_offset(z_offset){
        startCompression();
    }

// Start initial compression
void quadtree::startCompression(){
       int parent_z = parentBlockData.size();
       int parent_y = parentBlockData[0].size();
       int parent_x = parentBlockData[0][0].size();
       for(int z = 0; z < parent_z; z++){
           compressQuadrant(0, 0, z, parent_x, parent_y);
       }
}



;
// Recursively divide quadrants into 4 smaller quadrants until whole quadrant is uniform
void quadtree::compressQuadrant(int x, int y, int z, int width, int height){
    if(height == 1 && width == 1){
        printOutput(x,y,z,width,height);
        return;
    }

    if(isUniform(x,y,z,width, height)){
        printOutput(x,y,z,width,height);
        return;
    }
    
    if(width == 2 && height == 2){
        check2x2(x,y,z);
        return;
    }
    
    
    if(width == 1){
        printOutput(x,y,z,width,height);
        return;
    }
    if(height == 1){
        printOutput(x,y,z,width,height);
        return;
    }

    int halfWidth = width/2;
    int halfHeight = height/2;

    if(width%2 != 0){
        halfWidth += 1;
    }
    if(height%2 != 0){
        halfHeight += 1;
    }

    compressQuadrant(x, y, z, width/2, height/2);
    compressQuadrant(x+ width/2, y,z, halfWidth,height/2);
    compressQuadrant(x, y+ height/2,z, width/2,halfHeight);
    compressQuadrant(x + width/2, y + height/2,z, halfWidth,halfHeight);

}

//Checks if a quadrant is uniform
bool quadtree::isUniform(int x, int y, int z, int width, int height){
    char firstChar = parentBlockData[z][y][x];
        for (int i = y; i < y + height; i++){
            for (int j = x; j < x + width; j++){
                if(firstChar!= parentBlockData[z][i][j]){
                    return false;

                }
            }
        }
        return true;
}
    //std::cout<<"Z: "<<z<<"X: "<<x<<"Y: "<<y<<"Widht: "<<width<< "Height: "<< height;
void quadtree::check2x2(int x, int y, int z){
        bool bottomRowUniform = isUniform(x,y,z,2,1);
        bool topRowUniform = isUniform(x,y+1,z,2,1);
        bool leftColUniform = isUniform(x,y,z,1,2);
        bool rightColUniform = isUniform(x+1,y,z,1,2);

        if(bottomRowUniform && topRowUniform){
            printOutput(x,y,z,2,1);
            printOutput(x,y+1,z,2,1);
            return;
        }else if(leftColUniform && rightColUniform){
            printOutput(x,y,z,1,2);
            printOutput(x+1,y,z,1,2);
            return;
        }else if(bottomRowUniform){
            printOutput(x,y,z,2,1);
            printOutput(x,y+1,z,1,1);
            printOutput(x+1,y+1,z,1,1);
            return;
        }else if(topRowUniform){
            printOutput(x,y+1,z,2,1);
            printOutput(x,y,z,1,1);
            printOutput(x+1,y,z,1,1);
            return;
        }else if(leftColUniform){
            printOutput(x,y,z,1,2);
            printOutput(x+1,y,z,1,1);
            printOutput(x+1,y+1,z,1,1);
            return;
        }else if(rightColUniform){
            printOutput(x+1,y,z,1,2);
            printOutput(x,y,z,1,1);
            printOutput(x,y+1,z,1,1);
            return;
        }else{
            printOutput(x,y,z,1,1);
            printOutput(x,y+1,z,1,1);
            printOutput(x+1,y,z,1,1);
            printOutput(x+1,y+1,z,1,1);
            return;
        }

}

// Output in correct format
void quadtree::printOutput(int x, int y, int z, int width, int height){
    char tagVal = parentBlockData[z][y][x];
    std::string tagString = tagTable[tagVal];
    std::cout<<x+x_offset<<','<<y+y_offset<<','<<z+z_offset<<','<<width<<","<<height<<",1,"<<tagString<<std::endl;
}

