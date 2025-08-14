#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//so we want to convert the training images from the mnist dataset into
//an array of images, these images are 784 pixels 28x28, with colours from 0-255
// 



vector<int> convert_image_data(string file_name){
    

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
        count++;
    }

    // raw data now contains pixels from 60000 images, with images being seperated every 784 pixels
    // raw_data[image * 784 + row * 28 + col] to access each indivdual pixel   

    return raw_data;
}




int main(){

    string train_images_file_name ="Mnist_data/Train_data/train-images.idx3-ubyte"; 
    string test_images_file_name ="Mnist_data/Test_data/t10k-images.idx1-ubyte";

    train_data = convert_image_data(train_images_file_name);
    test_data = convert_image_data(test_images_file_name);




    return 0;
}