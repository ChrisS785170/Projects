/*

Basic introduction to Nueral networks and backpopagation

This projects goal is make a nueral network which can intake input data (hand written images) and return the correct answer for that data

A nueral network is a layers of nuerons linked together through weights and biases which calculate the probabilty the network believe the input actually is, this probability distribution is 
an array holding all the possible answers that the input could be and will guess the answer with the highest probability.

To begin with the program must convert the image into data for the network to intake, this is done in Mnist_input_translator.cpp, it basically converts the image into a data stream of 
pixel values which each image being 784 pixels long, this data stream contains values ranging from 0-1 describing the colour of the pixel;

Once the image is converted we feed images into the network's input layer, it then calculates the activation values for each layer in the network and finally calculates a probability distribution
for the final layer. it then calculates how bad the guesses were and over many samples alters the weights and biases so that it is more likely to guess the correct answer.
The pipeline goes in this order, input_conversion -> activation_calculation -> error calulation -> (after a batch of samples: default =20) Weight and bias update -> 
(after training data : default 60000) Test on unknown dataset

*/




#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Mnist_input_translator.cpp"
#include <cstdlib>
#include <random>
#include <cmath>

using namespace std;


/*
Default:
    layer size ( 784 -> 16 -> 16 -> 10)
    weight vector size ( [  [784[16]] ,  [16[16]] , [16[10]]   ])    weight[i] ( layer i)  weights[i][j] (layer i, jth node) weights[i][j][k] (layer i, jth node, kth weight)
    bias size (  [ [16] [16] [10]   ])
    error size (  [ [16] [16] [10]   ])
    layer_sizes = [784, 16,16, 10]
    z = [784, 16,16, 10]
    nueron = [784, 16,16, 10]

    Relu for activation calculation, then for output layer calculation Softmax
    He intialisation

*/

struct Model{

    /*The model we will be training */

    int number_of_hidden_layers;
    int number_of_input_nodes;
    int hidden_layer_size;
    vector<vector<vector<float>>> weights;
    vector<vector<float>> bias;
    vector<vector<float>> nuerons;
    vector<vector<float>> z;
    vector<vector<vector<float>>> weight_error;
    vector<vector<float>> error;
    vector<int> layer_sizes;
    float learning_rate;
    int label;
    int batch_size;
    float cost; 
};

void loss_function(vector<float> a, Model &AI){
    /*calculate cost from the output of the sample C = -sum(label[i]log(a[i]))    since label is just a bucnh of 0s it comes out to just be one calculation where label[i] = 1*/
    // this is never actually used in training so i didnt need to implement this : )
    AI.cost = -log(a[AI.label]);
}

float Relu(float num){
    /*standard relu function for activation calculation for every layer but the last*/

    if (num <= 0 ){
        return 0.0;
    }else{
        return num;
    }

}

vector<float> Softmax(vector<float> z){
    /*Softmax function for activation calculation for the final layer*/
    int sum = 0;
    vector<float> act = z;
    for (int i = 0; i < z.size(); i++){
        sum += exp(z[i]);
    }
  
    for (int i = 0; i < z.size(); i++){
        act[i] = exp(z[i])/sum; 
    }

    return act;
}


void activation_calculation(Model &AI , vector<float> input){
    /*calculate the activation for every nueron in every layer, begins by calculating the activation for every layer up to the final layer using relu and standard activation function
    a = relu(z)  z = w[i-1]*n[i-1] - bias[i-1]  where i is our layer
    final layer calculation is Softmax(z) 
    */

    AI.nuerons[0] = input;
    float sum;

    for (int i = 1; i < AI.layer_sizes.size()-1 ; i++){ // for each layer after the input layer  i.e loops from 1 - 2
        for (int j = 0; j < AI.layer_sizes[i]; j++){ // for each nueron in that layer
            sum = 0;
            for (int k = 0; k<AI.layer_sizes[i-1]; k++){ // for each weight in the previous layer
                    sum += AI.weights[i-1][k][j]* AI.nuerons[i-1][k]  - AI.bias[i-1][j] ; // bias is -1 as well as even tho its the bias of the second layer b[0] is the second layers bias     

            }
            AI.z[i][j] = sum;
            AI.nuerons[i][j] = Relu(sum);
             
        }
    }

    for (int j = 0; j < AI.layer_sizes.back(); j++){ // for final layer  
        sum = 0;
        for (int k = 0; k<AI.layer_sizes.size()-2; k++){ // for each weight in the second to last layer i.e layer 3 or w[2]
                sum += AI.weights[AI.layer_sizes.size()-2][k][j]* AI.nuerons[AI.layer_sizes.size()-2][k]  - AI.bias[AI.layer_sizes.size()-2][j] ; // bias is -1 as well as even tho its the bias of the second layer b[0] is the second layers bias     

        }
        AI.z[AI.layer_sizes.size()-1][j] = sum;            
    }
    AI.nuerons[AI.layer_sizes.size()-1] = Softmax(AI.z[AI.layer_sizes.size()-1]);

}



float derivitive_relu(float num){
    /*the derivitive of the relu function as its needed in the error calculation*/
    if (num <=  0){
        return 0 ; 

    }else{
        return 1.0;
    }
}
void update_AI(Model &AI){
    /*adjust weights and biases according to the error and batch size, then reset error for the next batch*/

    int batch_size  = AI.batch_size;

    for (int i = 0; i < AI.layer_sizes.size()-1 ; i++){ // for each layer after the input layer  will loop from 0 - 2
        for (int j = 0; j < AI.layer_sizes[i]; j++){ // for each nueron in that layer
            for (int k = 0; k<AI.layer_sizes[i+1]; k++){ // for each weight in that layer
       
                AI.weights[i][j][k] = AI.weights[i][j][k] - (AI.learning_rate*    ((AI.weight_error[i][j][k])/batch_size)   ); // need to make sure to divide the error by the batch size when we do the error calculation as it doesnt get done here
            }
        }
        for (int j = 0; j<AI.layer_sizes[i+1]; j++){
            AI.bias[i][j] = AI.bias[i][j] - (((AI.error[i][j])/batch_size) * AI.learning_rate); // the weight vector 

        }
    }

    for (int i = 0; i < AI.layer_sizes.size()-1; i++){ // 0 -2
        for (int j = 0; j<AI.layer_sizes[i+1]; j++ ){
            AI.error[i][j] = 0.0;
        }

    }


}


vector<vector<float>> transpose_weight_matrix(vector<vector<float>> weights){
    /* Transpose a matrix       [3,   4]   = [3 , 1]
                                [1 , 2]]     [4  , 2]  */
    vector<vector<float>> transpose = vector<vector<float>>  (weights[0].size(), vector<float>(weights.size(), 0.0)); // this doesnt work for different vector size 784 * 16 -> 16 * 784
    for (int i = 0; i< weights.size(); i++){
        for (int j = 0; j<weights[0].size(); j++){

            transpose[j][i] = weights[i][j];  // will have to test this later with smaller matrixes
        }
    }

    return transpose; 
}

vector<float> matrix_multiplication(vector<vector<float>> transpose , vector<float> error){
    /* just standard matrix multiplication done in maths */
    vector<float> result = vector<float> (transpose.size(), 0.0);
    float sum;
    for (int i = 0; i<transpose.size(); i++){
        sum = 0.0; 
        for (int j = 0; j<transpose[0].size(); j++){
            sum+= transpose[i][j]* error[j];
        }
        result[i] = sum;
    }
    return result;

}


void error_calculation(Model &AI){
    /*error calculation, used for adjusting weights each batch error is calculated each sample then summed over 20 samples and used to update the weights and biases 
    error calculation is different for weight and bias*/



    /*because of how the vector sizes out error and bias are -1 behind weights, nuerons and z for the same layer  */



    int output_layer_size = AI.layer_sizes.back(); 
    for (int i = 0; i<output_layer_size; i++){ // final layer calculation
        if (i == AI.label){
            AI.error[AI.layer_sizes.size()-2][i] += (AI.nuerons[AI.layer_sizes.size()-1][i] - 1 ) ; //AI.layer_sizes.size()-2 == 2 
        }else{
            AI.error[AI.layer_sizes.size()-2][i] += (AI.nuerons[AI.layer_sizes.size()-1][i] - 0 ) ;  
        }
    }
    vector<float> result;
    vector<vector<float>> transpose;
    for (int i = AI.layer_sizes.size()-3; i>0; i++){ // loop from second to last layer to the first start at 1 - 0  
        /*the way ive done the weight matrix might cause problems the with formula documentation in my implementation
        E[1] = W[1] * E[2] o R'(z[2])
        the issue comes with the transpose number of nuerons x weights to next nuerons
        */
        transpose = transpose_weight_matrix(AI.weights[i+1]); // i think this might be wrong 
        result = matrix_multiplication(transpose, AI.error[i+1]); // will begin at 3 and we want error 2

        for (int j = 0; j<AI.layer_sizes[i] ;j++){
            AI.error[i][j] = result[j] * derivitive_relu(AI.z[i+1][j]);
        } // since z has a size of 4, and we want the z of the third layer its [2] therfore i+1
    }



    for (int i = 0; i <AI.weights.size(); i++){ // 0 - 2
        for (int j = 0; j<AI.weights[i].size(); j++){ //  784 ->16 - > 16
            for (int k = 0; k<AI.weights[i][j].size(); k++){ // 16 - > 16 - > 10
                AI.weight_error[i][j][k] += AI.error[i][k] * AI.nuerons[i][j];
                
            }
        }
    }

}


void Heintialisation( vector<vector<vector<float>>> &weights ){  
    /*Heintialisation basically compares the number of input nodes of the layer and changes the range the weights could be intialised too. 
    Alg:  W = N(0 , sqrt(2/n)) where n is the number of inputs to the node  and N is the normal distribution with mean 0 and std deviation of sqrt(2/n)  */

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
    /*just intialising all the values in the Model, mainly all the vectors*/
    Model AI;
    
    vector<vector<vector<float>>> weights;
    for (int i = 0; i < layer_sizes.size()-1; i++){ // 0 - 2
        weights.push_back(vector<vector<float>>(layer_sizes[i], vector<float>(layer_sizes[i+1], 0.0)));
    }

    vector<vector<float>> bias;
    vector<vector<float>> error;
    vector<vector<float>> nuerons;
    vector<vector<float>> z;

    if (intialisation_alg == "default"){
        Heintialisation(weights);
        AI.weights = weights;


        for (int i = 0; i < layer_sizes.size()-1; i++){ // 0 -2
            bias.push_back(vector<float>(layer_sizes[i+1], 0.0));
            error.push_back(vector<float>(layer_sizes[i+1], 0.0));
        }
        for (int i = 0; i < layer_sizes.size(); i++){ //0 - 3
            nuerons.push_back(vector<float>(layer_sizes[i], 0.0));
            z.push_back(vector<float>(layer_sizes[i], 0.0));
        }
        AI.error = error;
        AI.bias = bias;
        AI.nuerons = nuerons;
        AI.layer_sizes = layer_sizes;
        AI.learning_rate = 0.01;
        AI.z = z;
    }

    return AI;
}

int guess_label(Model &AI){
    float max = 0;
    int guess = -1; 
    for (int i = 0; i<AI.layer_sizes.back(); i++){
        if (AI.nuerons[AI.layer_sizes.size()- 1][i] > max){
            max = AI.nuerons[AI.layer_sizes.size()- 1][i]; 
            guess = i;
        }
    }
    return guess;
}

void trainAI(){ 
    /*the general training funciton which wraps the whole process together */

    ////////////////////////////////////////////////////
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
    AI.batch_size =20;
    
    
    ///////////////////////////////////////////////////
    //Training
    ///////////////////////////////////////////////////
    int  train_dataset_size = ds.train_data.size() / 784; // 784 is the size of each image 28x28 pixels, will have to change this for modularity later 
    vector<float> input (input_layer_size , 0.0);
    int sample = 0;

    for (int i = 0; i < train_dataset_size; i++){ // should be 60000 for Mnist dataset
        AI.label = ds.train_labels[sample];
        for (int j = 0; j<input_layer_size; j++){
            input[j] = ds.train_data[j + (sample*input_layer_size)];
        }
        if (sample%20 == 0){ // do an update every 20 samples
            activation_calculation(AI, input);
            error_calculation(AI);
            update_AI(AI);

        }else{
            activation_calculation(AI, input);
            error_calculation(AI);
        }
        sample++;
        


    }
    ///////////////////////////////////////////////////
    // Testing
    ///////////////////////////////////////////////////

    int test_dataset_size = ds.test_data.size() / 784;
    sample = 0;
    int guess = 0;
    int number_of_correct_guesses= 0;
    for (int i = 0; i < test_dataset_size; i++){
        AI.label = ds.test_labels[sample];
        for (int j = 0; j<input_layer_size; j++){
            input[j] = ds.test_data[j + (sample*input_layer_size)];
        }

        activation_calculation(AI, input); 
        guess  = guess_label(AI);
        if (guess == AI.label){
            number_of_correct_guesses++;
        }
    }
    float percentage_of_correct_guesses = number_of_correct_guesses/test_dataset_size; 
    cout<<"percentage of correct guesses :"<< percentage_of_correct_guesses<< "/n";

}




int main(){
    return 0;
}