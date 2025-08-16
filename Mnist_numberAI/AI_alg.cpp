#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Mnist_input_translator.cpp>
#include <cstdlib>

using namespace std;

// Plan is to implement back propagation, sorting of images, overall training process.

struct Model{
    vector<vector<vector<float>>> weights;
    vector<vector<float>> bias;
};

srand(time(0));

float Randomweight(){ // return a random number between -1 and 1, for intialising weights 

    float num = rand()%201;
    num = (num - 100)/100;

    return num;

}

float RandomBias(){
    float num = rand()%2
    return num; 
}

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