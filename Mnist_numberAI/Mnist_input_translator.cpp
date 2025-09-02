#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/*
 * File: Mnist_input_translator
 * ------------------------
 * This file provides utility functions for loading the MNIST dataset
 * (images and labels) from the standard IDX binary file format into
 * usable C++ data structures.
 *
 * Overview:
 *   - The MNIST dataset consists of 28x28 grayscale images of handwritten
 *     digits (0–9). Each pixel is stored as an unsigned byte (0–255).
 *   - The dataset comes with corresponding label files, where each entry
 *     is a single digit (0–9).
 *
 * Functions:
 *   - convert_image_data(file_name):
 *       Reads an IDX image file, skips the 16-byte header, and converts
 *       each pixel to a normalized float (0.0–1.0). Returns a flat vector
 *       containing all images in sequence.
 *
 *   - convert_label_data(file_name):
 *       Reads an IDX label file, skips the 8-byte header, and extracts
 *       digit labels as integers (0–9). Returns a vector of labels aligned
 *       with the images.
 *
 *   - data_setup():
 *       Wrapper function that loads all training and test data from the
 *       MNIST dataset (images + labels). Populates and returns a Dataset
 *       struct containing:
 *          • train_data   (flattened normalized images for training)
 *          • test_data    (flattened normalized images for testing)
 *          • train_labels (digit labels for training data)
 *          • test_labels  (digit labels for test data)
 *
 * Usage:
 *   Dataset ds = data_setup();
 *   // ds now contains all MNIST training and testing data ready for use
 *   // in a neural network or machine learning model.
 */


struct Dataset {
    /*
    * Struct: Dataset
    * ---------------
    * A container for holding the MNIST dataset in memory.
    * 
    * Members:
    *   - train_data  : Flattened pixel values for the training set.
    *                   Each image is stored as a vector<float> of size 784 (28x28).
    *                   Values are typically normalized (e.g., between 0 and 1).
    *
    *   - test_data   : Flattened pixel values for the test set, stored in the same
    *                   format as train_data.
    *
    *   - train_labels: Integer class labels (0–9) corresponding to each training image.
    *
    *   - test_labels : Integer class labels (0–9) corresponding to each test image.
    *
    * Notes:
    *   - This struct is a simple way to keep training and test sets grouped
    *     together and passed around the program.
    */
    vector<float> train_data;   // Training image data (flattened)
    vector<float> test_data;    // Test image data (flattened)
    vector<int>  train_labels;  // Training labels (0–9)
    vector<int>  test_labels;   // Test labels (0–9)
};



vector<float> convert_image_data(string file_name) {
    /*
    * Function: convert_image_data
    * ----------------------------
    * Reads MNIST image data from a binary file and converts it into a normalized
    * vector of floats.
    *
    * Parameters:
    *   - file_name : The path to the MNIST image file in IDX format (e.g.,
    *                 "train-images.idx3-ubyte" or "t10k-images.idx3-ubyte").
    *
    * Process:
    *   1. Opens the file in binary mode.
    *   2. Skips the first 16 bytes (the IDX image header).
    *   3. Reads each subsequent byte representing a pixel (0–255 grayscale).
    *   4. Converts each byte into a float in the range [0, 1] by dividing by 255.
    *   5. Stores the result in raw_data.
    *
    * Returns:
    *   - A vector<float> containing the normalized pixel data for all images
    *     in the file, flattened into a single continuous array.
    *
    * Notes:
    *   - Each image is 28x28 pixels (784 floats).
    *   - Images are stored back-to-back in the output vector.
    *   - To access a pixel:
    *       raw_data[image_index * 784 + row * 28 + col]
    *   - Example: For image `i`, pixel at row `r` and column `c`:
    *       pixel_value = raw_data[i * 784 + r * 28 + c];
    */
    // Open the file in binary mode
    ifstream file(file_name, ios::binary);
    unsigned char byte;
    int count = 0;
    vector<float> raw_data;

    // Skip the first 16 bytes (header), then read pixel data
    // Normalize pixel values to [0, 1] for use in the AI
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (count > 15) {
            raw_data.push_back(static_cast<float>(byte) / 255.0f);
        } else {
            count++;
        }
    }

    // raw_data now contains all images flattened into a single vector
    // Example access: raw_data[image * 784 + row * 28 + col]

    return raw_data;
}


vector<int> convert_label_data(string file_name) {   
    /*
    * Function: convert_label_data
    * ----------------------------
    * Reads MNIST label data from a binary file and stores it in a vector of ints.
    *
    * Parameters:
    *   - file_name : The path to the MNIST label file in IDX format
    *                 (e.g., "train-labels.idx1-ubyte" or "t10k-labels.idx1-ubyte").
    *
    * Process:
    *   1. Opens the file in binary mode.
    *   2. Skips the first 8 bytes (the IDX label header).
    *   3. Reads each subsequent byte, which corresponds to an image label.
    *   4. Casts the byte into an int (range 0–9).
    *   5. Appends each label into raw_data.
    *
    * Returns:
    *   - A vector<int> containing all labels, where raw_data[i] corresponds to
    *     the label for the i-th image.
    *
    * Notes:
    *   - Each label is a single number between 0 and 9.
    *   - Example: raw_data[0] is the label for the first image.
    */

    // Open the file in binary mode
    ifstream file(file_name, ios::binary);
    unsigned char byte;
    int count = 0;
    vector<int> raw_data;

    // Skip the 8-byte header, then read each label
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (count > 7) { 
            raw_data.push_back(static_cast<int>(byte));
        } else {
            count++;
        }
    }

    // raw_data now contains one label per image (values 0–9)
    return raw_data;
}



Dataset data_setup() {

    /*
    * Function: data_setup
    * --------------------
    * Loads the MNIST dataset (both images and labels for training and testing)
    * into a Dataset struct.
    *
    * Process:
    *   1. Declares a Dataset object to hold all data.
    *   2. Defines file paths for the MNIST training and testing image/label files.
    *   3. Uses convert_image_data() to load and normalize pixel data from the IDX image files.
    *   4. Uses convert_label_data() to load labels (0–9) from the IDX label files.
    *   5. Stores the loaded data in the Dataset struct.
    *
    * Returns:
    *   - A fully populated Dataset struct containing:
    *       ds.train_data   -> vector<float> of all training images (normalized pixel values).
    *       ds.test_data    -> vector<float> of all testing images (normalized pixel values).
    *       ds.train_labels -> vector<int>   of training labels (0–9).
    *       ds.test_labels  -> vector<int>   of testing labels (0–9).
    *
    * Notes:
    *   - Each image is 28x28 pixels (flattened to 784 values).
    *   - Training set: 60,000 images and labels.
    *   - Test set: 10,000 images and labels.
    */
    Dataset ds;

    // Initialize all data containers (not strictly necessary but explicit)
    vector<float> train_data;
    vector<float> test_data;
    vector<int> train_labels;
    vector<int> test_labels;

    // File paths for the MNIST dataset
    string train_images_file_name = "Mnist_data/Train_data/train-images.idx3-ubyte"; 
    string test_images_file_name  = "Mnist_data/Test_data/t10k-images.idx3-ubyte";
    string train_labels_file_name = "Mnist_data/Train_data/train-labels.idx1-ubyte"; 
    string test_labels_file_name  = "Mnist_data/Test_data/t10k-labels.idx1-ubyte";

    // Load and normalize image data
    ds.train_data = convert_image_data(train_images_file_name);
    ds.test_data  = convert_image_data(test_images_file_name);

    // Load label data
    ds.train_labels = convert_label_data(train_labels_file_name);
    ds.test_labels  = convert_label_data(test_labels_file_name);

    // Return the fully populated dataset
    return ds;
}
