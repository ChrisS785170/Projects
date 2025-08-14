#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/*
    TESTING
    - check arrays are correctly converted and configured
    - check labels match images

*/




//so we want to convert the training images from the mnist dataset into
//an array of images, these images are 784 pixels 28x28, with colours from 0-255
// we want to have an array with images with colour ranges from 0-1 and labels which hold the value the images shows



vector<float> convert_image_data(string file_name){

    // read file from file name
    ifstream file(file_name, ios::binary);
    unsigned char byte;
    int count = 0;
    vector<float> raw_data;

    // we skip the first 16 values as it is the header, then put all the data into rawdata
    // cast it from a byte to a float then divide by 255 to get 0-1 ranges needed for AI
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (count > 15){
            raw_data.push_back(static_cast<float>(byte)/255);
        }
        else{
            count++;
        }
    }

    // raw data now contains pixels from 60000 images, with images being seperated every 784 pixels
    // raw_data[image * 784 + row * 28 + col] to access each indivdual pixel   

    return raw_data;
}

// creates an array with the images labels, for example label for image 1 is label[0]. should be between 0-9
vector<int> convert_label_data(string file_name){

    ifstream file(file_name, ios::binary);
    unsigned char byte;
    int count = 0;
    vector<int> raw_data;

    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (count > 15){
            raw_data.push_back(static_cast<int>(byte));
        }
        else{
            count++;
        }
    }

    return raw_data;
}


int main(){
    // intialise all data containers
    vector<float> train_data;
    vector<float> test_data;
    vector<int> train_labels;
    vector<int> test_labels;
    // the file paths for the data
    string train_images_file_name ="Mnist_data/Train_data/train-images.idx3-ubyte"; 
    string test_images_file_name ="Mnist_data/Test_data/t10k-images.idx3-ubyte";
    string train_labels_file_name ="Mnist_data/Train_data/train-labels.idx1-ubyte"; 
    string test_labels_file_name ="Mnist_data/Test_data/t10k-labels.idx1-ubyte";


    train_data = convert_image_data(train_images_file_name);
    test_data = convert_image_data(test_images_file_name);
    train_labels = convert_label_data(train_labels_file_name);
    test_labels = convert_label_data(test_labels_file_name);








    return 0;
}