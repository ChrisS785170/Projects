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

/*
float Randomweight(){ // return a random number between -1 and 1, for intialising weights 
    srand(time(0));
    float num = rand()%201;
    num = (num - 100)/100;

    return num;

}

float RandomBias(){
    srand(time(0));
    float num = rand()%2;
    return num; 
}
*/
Model intialisation(int number_of_hidden_layers, int input_layer_size , int hidden_layer_size , int output_layer_size, string intialisation_alg ){
    Model AI;
    
    
    
    if (intialisation_alg.empty()){
        intialisation_alg = "default";
    }

    if (intialisation_alg == "default"){


    }
    


    

    // make random inputs


    return AI;
}

int trainAI(){

    int input_layer_size = 784; // 28x28 image size
    int hidden_layer_size = 16;
    int number_of_hidden_layers = 2;
    int output_layer_size = 10;


    Dataset ds;
    ds = data_setup();
    Model AI = intialisation(number_of_hidden_layers, input_layer_size, hidden_layer_size, output_layer_size, "default");


    return 0;
}




int main(){
    return 0;
}