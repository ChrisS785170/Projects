#include "octree.h"
// Quadtree compession algorithms, recursively divides slices into 4 smaller quadtrants and checks
// if uniform. 
// testFiles/test_data_file_32768_4x4x4.csv
// testFiles/the_fast_one_376000_2x2x2.csv
// x86_64-w64-mingw32-g++ -static -o unitek-octree.exe main.cpp streamProcessor/streamProcessor.cpp 
// compressionAlgorithms/quadtree.cpp compressionAlgorithms/noCompression.cpp compressionAlgorithms/RLECompression.cpp
// compressionAlgorithms/octree.cpp
// g++ -std=c++11 -Wall -g main.cpp streamProcessor/streamProcessor.cpp compressionAlgorithms/noCompression.cpp 
// compressionAlgorithms/RLECompression.cpp compressionAlgorithms/quadtree.cpp -o main

// Constructor to assign variables
octree::octree(const std::vector<std::vector<std::vector<char> > >& parentBlockData,
                             const std::unordered_map<char, std::string>& tagTable,
                             int headerValues[6], int x_offset, int y_offset, int z_offset)
    : parentBlockData(parentBlockData), tagTable(tagTable), 
    x_offset(x_offset), y_offset(y_offset), z_offset(z_offset){
        startCompression();
    }

// Start initial compression
void octree::startCompression(){
       int parent_z = parentBlockData.size();
       int parent_y = parentBlockData[0].size();
       int parent_x = parentBlockData[0][0].size();

        compressOctant(0, 0, 0, parent_x, parent_y, parent_z);
       
}

struct Block{
    char character;
    int x,y,z;
    int width, height, depth;
};




// Recursively divide quadrants into 8 smaller quadrants until whole quadrant is uniform
void octree::compressOctant(int x, int y, int z, int width, int height, int depth){

    

    if(width <= 1 && height <= 1 && depth <= 1){
        printOutput(x,y,z,width,height, depth);
        return;
    }
    //std::cout<<"Width: "<<width<< " Height: "<<height<<" Depth: "<<depth<<std::endl;
    if( (width == 1 && height == 1 && depth == 2) ||
        (width == 1 && height == 2 && depth == 1) ||
        (width == 2 && height == 1 && depth == 1)){
        printOutput(x,y,z,width,height, depth);
        return;
    }
    

    if(isUniform(x,y,z,width,height, depth) != 0){
        printOutput(x,y,z,width,height, depth);
        return;
    }

    int halfWidth = width / 2;
    int halfHeight = height / 2;
    int halfDepth = depth / 2;

 
    if(width >= 2 && height >= 2 && depth >= 2){
        char c1, c2, c3, c4, c5, c6, c7, c8;
        c1 = isUniform(x, y, z, halfWidth, halfHeight, halfDepth); 
        c2 = isUniform(x + halfWidth, y, z, width - halfWidth, halfHeight, halfDepth); 
        c3 = isUniform(x, y + halfHeight, z, halfWidth, height - halfHeight, halfDepth); 
        c4 = isUniform(x + halfWidth, y + halfHeight, z, width - halfWidth, height - halfHeight, halfDepth); 
        c5 = isUniform(x, y, z + halfDepth, halfWidth, halfHeight, depth - halfDepth); 
        c6 = isUniform(x + halfWidth, y, z + halfDepth, width - halfWidth, halfHeight, depth - halfDepth); 
        c7 = isUniform(x, y + halfHeight, z + halfDepth, halfWidth, height - halfHeight, depth - halfDepth); 
        c8 = isUniform(x + halfWidth, y + halfHeight, z + halfDepth, width - halfWidth, height - halfHeight, depth - halfDepth); 
    
        
        std::vector<Block> subBlocks;
        if(c1 != 0) subBlocks.push_back({c1, x, y, z, halfWidth, halfHeight, halfDepth});
        if(c2 != 0) subBlocks.push_back({c2, x + halfWidth, y, z, width - halfWidth, halfHeight, halfDepth});
        if(c3 != 0) subBlocks.push_back({c3, x, y + halfHeight, z, halfWidth, height - halfHeight, halfDepth});
        if(c4 != 0) subBlocks.push_back({c4, x + halfWidth, y + halfHeight, z, width - halfWidth, height - halfHeight, halfDepth});
        if(c5 != 0) subBlocks.push_back({c5, x, y, z + halfDepth, halfWidth, halfHeight, depth - halfDepth});
        if(c6 != 0) subBlocks.push_back({c6, x + halfWidth, y, z + halfDepth, width - halfWidth, halfHeight, depth - halfDepth});
        if(c7 != 0) subBlocks.push_back({c7, x, y + halfHeight, z + halfDepth, halfWidth, height - halfHeight, depth - halfDepth});
        if(c8 != 0) subBlocks.push_back({c8, x + halfWidth, y + halfHeight, z + halfDepth, width - halfWidth, height - halfHeight, depth - halfDepth});

        merge(subBlocks);

        for(const auto a : subBlocks){
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
        }


        if (c1 == 0) compressOctant(x, y, z, halfWidth, halfHeight, halfDepth);
        if (c2 == 0) compressOctant(x + halfWidth, y, z, width - halfWidth, halfHeight, halfDepth);
        if (c3 == 0) compressOctant(x, y + halfHeight, z, halfWidth, height - halfHeight, halfDepth);
        if (c4 == 0) compressOctant(x + halfWidth, y + halfHeight, z, width - halfWidth, height - halfHeight, halfDepth);
        if (c5 == 0) compressOctant(x, y, z + halfDepth, halfWidth, halfHeight, depth - halfDepth);
        if (c6 == 0) compressOctant(x + halfWidth, y, z + halfDepth, width - halfWidth, halfHeight, depth - halfDepth);
        if (c7 == 0) compressOctant(x, y + halfHeight, z + halfDepth, halfWidth, height - halfHeight, depth - halfDepth);
        if (c8 == 0) compressOctant(x + halfWidth, y + halfHeight, z + halfDepth, width - halfWidth, height - halfHeight, depth - halfDepth);
        return;
    }
    if(width == 2 && height == 2 && depth == 1){
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z][y][x+1];
        char c3 = parentBlockData[z][y+1][x];
        char c4 = parentBlockData[z][y+1][x+1];

        std::vector<Block> subBlocks;
        subBlocks.push_back({c1, x, y, z, 1, 1, 1});
        subBlocks.push_back({c2, x + 1, y, z, 1, 1, 1});
        subBlocks.push_back({c3, x, y + 1, z, 1, 1, 1});
        subBlocks.push_back({c4, x + 1, y + 1, z, 1, 1, 1});

        merge(subBlocks);
        for(const auto a : subBlocks){
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
        }
        return;
    }
    if (width == 2 && height == 1 && depth == 2) {
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z][y][x + 1];
        char c3 = parentBlockData[z + 1][y][x];
        char c4 = parentBlockData[z + 1][y][x + 1];

        std::vector<Block> subBlocks;
        subBlocks.push_back({c1, x, y, z, 1, 1, 1});
        subBlocks.push_back({c2, x + 1, y, z, 1, 1, 1});
        subBlocks.push_back({c3, x, y, z + 1, 1, 1, 1});
        subBlocks.push_back({c4, x + 1, y, z + 1, 1, 1, 1});

        merge(subBlocks);
        for (const auto &block : subBlocks) {
            printOutput(block.x, block.y, block.z, block.width, block.height, block.depth);
        }
        return;
    }
    if (width == 1 && height == 2 && depth == 2) {
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z + 1][y][x];
        char c3 = parentBlockData[z][y + 1][x];
        char c4 = parentBlockData[z + 1][y + 1][x];

        std::vector<Block> subBlocks;
        subBlocks.push_back({c1, x, y, z, 1, 1, 1});
        subBlocks.push_back({c2, x, y, z + 1, 1, 1, 1});
        subBlocks.push_back({c3, x, y + 1, z, 1, 1, 1});
        subBlocks.push_back({c4, x, y + 1, z + 1, 1, 1, 1});

        merge(subBlocks);
        for (const auto &block : subBlocks) {
            printOutput(block.x, block.y, block.z, block.width, block.height, block.depth);
        }
        return;
    }

    if(width == 2 && height == 1 && depth == 1){
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z][y][x+1];

        Block a = {c1, x, y, z, 1, 1, 1};
        Block b = {c2, x + 1, y, z, 1, 1, 1};


        if(tryMerge(a,b)){
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
        }else{
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
            printOutput(b.x, b.y, b.z, b.width, b.height, b.depth);
        }
        return;
    }
    if (width == 1 && height == 2 && depth == 1) {
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z][y + 1][x];

        Block a = {c1, x, y, z, 1, 1, 1};
        Block b = {c2, x, y + 1, z, 1, 1, 1}; 

        if (tryMerge(a, b)) {
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
        } else {
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
            printOutput(b.x, b.y, b.z, b.width, b.height, b.depth);
        }
        return;
    }
    if (width == 1 && height == 1 && depth == 2) {
        char c1 = parentBlockData[z][y][x];
        char c2 = parentBlockData[z + 1][y][x];

        Block a = {c1, x, y, z, 1, 1, 1};
        Block b = {c2, x, y, z + 1, 1, 1, 1}; 

        if (tryMerge(a, b)) {
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
        } else {
            printOutput(a.x, a.y, a.z, a.width, a.height, a.depth);
            printOutput(b.x, b.y, b.z, b.width, b.height, b.depth);
        }
        return;
    }
    return;
}


//Checks if a quadrant is uniform
char octree::isUniform(int x, int y, int z, int width, int height, int depth){

    char firstChar = parentBlockData[z][y][x];
    for(int i = z; i< z+depth ; i++){
        for (int j = y; j < y + height; j++){
            for (int k = x; k < x + width; k++){
                if (parentBlockData[i][j][k]!= firstChar) {
                return 0;
                }
            }
        }
    }
    return firstChar;
}

void octree::merge(std::vector<Block>& subBlocks){
    bool merged = true;
    while(merged){
        merged = false;
        for(int i = 0; i < subBlocks.size(); i++){
            for(int j = i+1; j < subBlocks.size(); j++){
                if(tryMerge(subBlocks[i], subBlocks[j])){
                    subBlocks.erase(subBlocks.begin() + j);
                    merged = true;
                    break;
                    j--;
                }
            }
            if(merged){
                break;
            }
        }
    }
}
bool octree::tryMerge(Block& a, Block& b){
    if(a.character == b.character){
        if(a.x == b.x && a.y == b.y && a.z + a.depth == b.z){
            if(a.width == b.width && a.height == b.height){
                a.depth += b.depth;
                return true;
            }
        }
        if(a.x == b.x && a.y + a.height == b.y && a.z == b.z){
            if(a.width == b.width && a.depth == b.depth){
                a.height += b.height;
                return true;
            }
        }
        if(a.x + a.width == b.x && a.y == b.y && a.z == b.z){
            if(a.depth == b.depth && a.height == b.height){
                a.width += b.width;
                return true;
            }
        }
    }
    return false;
}


// Output in correct format
void octree::printOutput(int x, int y, int z, int width, int height, int depth){
    char tagVal = parentBlockData[z][y][x];
    std::string tagString = tagTable[tagVal];
    std::cout<<x+x_offset<<','<<y+y_offset<<','<<z+z_offset<<','<<width<<","<<height<<","<<depth<<","<<tagString<<std::endl;
}