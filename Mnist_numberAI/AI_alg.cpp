#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Mnist_input_translator.cpp"
#include <cstdlib>
#include <random>

using namespace std;

// Plan is to implement back propagation, sorting of images, overall training process.


/*
Default:
    layer size ( 784 -> 16 -> 16 -> 10)
    weight vector size ( [  [784[16]] ,  [16[16]] , [16[10]]   ])    weight[i] ( layer i)  weights[i][j] (layer i, jth node) weights[i][j][k] (layer i, jth node, kth weight)
    bias size (  [ [16] [16] [10]   ])

    Relu (maybe leaky Relu to avoid problems) for activation calculation, then for output layer calculation Softmax
    Will use He intialisation

*/

struct Model{
    int number_of_hidden_layers;
    int number_of_input_nodes;
    int hidden_layer_size;
    vector<vector<vector<float>>> weights;
    vector<vector<float>> bias;
};



// W = N(0 , sqrt(2/n)) where n is the number of inputs to the node 
void Heintialisation( vector<vector<vector<float>>> &weights ){  

    for (int i = 0; i<weights.size(); i++){ // for the number layers
        float std = sqrt(2/weights[i].size());

        
        default_random_engine generator; // might want to add a seed
        normal_distribution<double> distribution (0.0,std);

        for (int j = 0; j<weights[i].size(); j++){ // for every node in each layer
            for (int k = 0; k<weights[i][0].size(); k++){ // for every weight in each node
                weights[i][j][k] = distribution(generator);
            }
        }


    }



}


Model intialisation(vector<int> layer_sizes, string intialisation_alg ){
    Model AI;
    
    vector<vector<vector<float>>> weights;
    for (int i = 0; i < layer_sizes.size()-1; i++){
        weights.push_back(vector<vector<float>>(layer_sizes[i], vector<float>(layer_sizes[i+1], 0.0)));
    }
    Heintialisation(&weights);
    AI.weights = weights;

    vector<vector<float>> bias;

    if (intialisation_alg == "default"){
        Heintialisation(&weights);
        AI.weights = weights;


        for (int i = 0; i < layer_sizes.size()-1; i++){
            bias.push_back(vector<float>(layer_sizes[i+1], 0.0))
        }

        AI.bias = bias;
    }

    return AI;
}

int trainAI(){ // parameters would be layers and specificed alg 


    // intialisation
    ////////////////////////////////////////////////////
    int input_layer_size = 784; // 28x28 image size
    vector<int> hidden_layer_size (2,16);
    int number_of_hidden_layers = 2;
    int output_layer_size = 10;

    vector<int> layer_size = {input_layer_size};
    for (int i = 0; i < hidden_layer_size.size(); i++){
        layer_size.push_back(hidden_layer_size[i]);
    }
    layer_size.push_back(output_layer_size);

    Dataset ds;
    ds = data_setup();
    Model AI = intialisation( layer_size, "default");
    
    ///////////////////////////////////////////////////

    //Training
    ///////////////////////////////////////////////////
    int  train_dataset_size = ds.train_data / 784 // 784 is the size of each image 28x28 pixels

    for (int i = 0; i < train_dataset_size; i++){ // should be 60000 for Mnist dataset

        // need to calculate activation of each layer - > softmax output layer - > calulate cost -> calculate error -> backpropagate
    }



    ///////////////////////////////////////////////////


    return 0;
}




int main(){
    return 0;
}