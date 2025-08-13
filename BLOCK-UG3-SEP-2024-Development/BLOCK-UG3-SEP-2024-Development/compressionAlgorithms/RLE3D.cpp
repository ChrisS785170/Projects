#include "RLE3D.h"

// Constructor
RLE3D::RLE3D(
    const std::vector<std::vector<std::vector<char>>>& parentBlockData,
    const std::unordered_map<char, std::string>& tagTable,
    int headerValues[6], int x_offset, int y_offset, int z_offset)
    : parentBlockData(parentBlockData), tagTable(tagTable),
      x_offset(x_offset), y_offset(y_offset), z_offset(z_offset) {
    startCompression();
}

// Start compression on the parent block data
void RLE3D::startCompression() {
    int parent_x_size = parentBlockData[0][0].size();
    int parent_y_size = parentBlockData[0].size();
    int parent_z_size = parentBlockData.size();

    // Initialize processed array
    std::vector<std::vector<std::vector<bool>>> processed(
        parent_z_size, std::vector<std::vector<bool>>(
            parent_y_size, std::vector<bool>(parent_x_size, false)));

    // Iterate over all voxels in the parent block
    for (int z = 0; z < parent_z_size; z++) {
        for (int y = 0; y < parent_y_size; y++) {
            for (int x = 0; x < parent_x_size; x++) {
                if (processed[z][y][x]) continue;

                char currentTag = parentBlockData[z][y][x];

                // Determine x_size
                int x_size = 1;
                while (x + x_size < parent_x_size &&
                       !processed[z][y][x + x_size] &&
                       parentBlockData[z][y][x + x_size] == currentTag) {
                    x_size++;
                }

                // Determine y_size
                int y_size = 1;
                bool expandY = true;
                while (expandY && y + y_size < parent_y_size) {
                    for (int xi = 0; xi < x_size; xi++) {
                        if (processed[z][y + y_size][x + xi] ||
                            parentBlockData[z][y + y_size][x + xi] !=
                                currentTag) {
                            expandY = false;
                            break;
                        }
                    }
                    if (expandY) y_size++;
                }

                // Determine z_size
                int z_size = 1;
                bool expandZ = true;
                while (expandZ && z + z_size < parent_z_size) {
                    for (int yi = 0; yi < y_size; yi++) {
                        for (int xi = 0; xi < x_size; xi++) {
                            if (processed[z + z_size][y + yi][x + xi] ||
                                parentBlockData[z + z_size][y + yi][x + xi]
                                    != currentTag) {
                                expandZ = false;
                                break;
                            }
                        }
                        if (!expandZ) break;
                    }
                    if (expandZ) z_size++;
                }

                // Mark voxels as processed
                for (int zi = 0; zi < z_size; zi++) {
                    for (int yi = 0; yi < y_size; yi++) {
                        for (int xi = 0; xi < x_size; xi++) {
                            processed[z + zi][y + yi][x + xi] = true;
                        }
                    }
                }

                // Store the compressed block with global offsets
                compressedBlocks.push_back({
                    x + x_offset, y + y_offset, z + z_offset,
                    x_size, y_size, z_size, currentTag});
            }
        }
    }

    startOutput();
}

// Output the compressed blocks
void RLE3D::startOutput() {
    for (const Block& block : compressedBlocks) {
        auto it = tagTable.find(block.tag);
        std::string label = (it != tagTable.end()) ? it->second : "";
        std::cout << block.x << ',' << block.y << ',' << block.z << ','
                  << block.x_size << ',' << block.y_size << ','
                  << block.z_size << ',' << label << std::endl;
    }
}