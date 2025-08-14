#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Mnist_input_translator.cpp>

using namespace std;

// Plan is to implement back propagation, sorting of images, overall training process.

struct Model{
    vector<vector<vector<float>>> weights;
    vector<vector<float>> bias;
};


Model intialisation(int number_of_hidden_layers, int input_layer_size , int hidden_layer_size  ){
    Model AI;
    // make random inputs


    return AI;
}

int trainAI(){
    Dataset ds;
    ds = data_setup();
    //Model AI = intialisation( );


    return 0;
}