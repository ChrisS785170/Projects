#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Mnist_input_translator.cpp"
using namespace std;


// need to make a binary file that has pre designed image and label and see the behaviour of input function 
//needs to mimic the actual mnist files but not on such a large scale



void make_test1ubyte(){
    std::ofstream f("small_test_file.ubyte", std::ios::binary);

    unsigned char header[] = {
        0x00, 0x00, 0x08, 0x03, // magic
        0x00, 0x00, 0x00, 0x01, // num images = 1
        0x00, 0x00, 0x00, 0x02, // rows = 2
        0x00, 0x00, 0x00, 0x02  // cols = 2
    };
    f.write((char*)header, sizeof(header));

    unsigned char pixels[] = {10, 20, 30, 40};
    f.write((char*)pixels, sizeof(pixels));

    f.close();
}

void make_test3ubyte(){
    std::ofstream f("small_test3_file.ubyte", std::ios::binary);

    unsigned char header[] = {
        0x00, 0x00, 0x08, 0x01, // magic: unsigned byte, 1 dimension
        0x00, 0x00, 0x00, 0x03  // num labels = 3
    };

    unsigned char labels[] = { 5, 0, 7 };
    f.write((char*)header, sizeof(header));

    f.write((char*)labels, sizeof(labels));

    f.close();
}
/*
void make_test2ubyte(){
    std::ofstream f("small_test2_file.ubyte", std::ios::binary);

    unsigned char header[] = {
        0x00, 0x00, 0x08, 0x03, // magic
        0x00, 0x00, 0x00, 0x01, // num images = 1
        0x00, 0x00, 0x00, 0x02, // rows = 2
        0x00, 0x00, 0x00, 0x02  // cols = 2
    };
    f.write((char*)header, sizeof(header));

    unsigned char pixels[784] 

    for (int i = 0; i<784; i++){
        if( i !=20 | i !=300 | i !=500 ){
            pixels[i] = 0;
        }
        else{
            pixels[i]= 255;
        }
    }

    f.write((char*)pixels, sizeof(pixels));

    f.close();
}

*/

int main() {
    make_test1ubyte();
    make_test3ubyte();

    vector<int>test3ans = { 5, 0, 7 };

    vector<float> test1ans = {10, 20,30,40};
    for (int i = 0; i<4; i++){
        test1ans[i] = test1ans[i]/255;
    }

    
    
    //for (int i = 0; i<784; i++){
    //    if( i !=20 | i !=300 | i !=500 ){
    //        test2ans[i] = 0;
    //    }
    //    else{
    //        test2ans[i]= 255;
    //    }
    //}



    //make_test2ubyte();

    //vector<float> train_data2;

    string filename = "small_test_file.ubyte";
    string filename3 = "small_test3_file.ubyte";
    //string filename2 = "small_test2_file.ubyte";
    vector<float> train_data = convert_image_data(filename);
    //train_data2 = convert_image_data(filename2);
    vector<int> train_labels = convert_label_data(filename3);



    if (train_data == test1ans){
        cout<< "input test 1 passed"<<"\n";
    }
    else{
        cout<<"input test 1 failed"<<"\n";

        for (int i = 0; i<4; i++){
            cout<<train_data[i]<< " | "<<test1ans[i]<< "\n";
        }
    }
///////////////////////////////////
    if (train_labels == test3ans){
        cout<< "input test 3 passed"<<"\n";
    }
    else{
        cout<<"input test 3 failed"<<"\n";

        for (int i = 0; i<3; i++){
            cout<<train_labels[i]<< " | "<<test3ans[i]<< "\n";
        }
    }

    /*
    if (train_data2 == test2ans){
        cout<< "input test 1 passed"<<"\n";
    }
    else{
        cout<<"input test 1 failed"<<"\n";

        for (int i = 0; i<784; i++){
            cout<<train_data[i]<< " | "<<test1ans[i]<< "\n";
        }
    }
    */
}
