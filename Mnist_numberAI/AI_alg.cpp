#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Mnist_input_translator.cpp"
#include <cstdlib>
#include <random>
#include <cmath>

using namespace std;


/**
 * @file AI_alg.cpp
 * @brief Implementation of a simple feedforward neural network for MNIST digit classification.
 *
 * This file contains the full workflow for setting up, training, and testing
 * a neural network model from scratch without external ML libraries.
 *
 * -----------------------
 * High-level overview:
 * -----------------------
 * 1. **Initialization**
 *    - Network architecture is defined by input, hidden, and output layer sizes.
 *    - Weights and biases are initialized using He initialization to improve convergence.
 *
 * 2. **Training**
 *    - Training data is loaded from MNIST ubyte files.
 *    - Each sample is passed forward through the network (`activation_calculation`).
 *    - Errors are computed against the true label (`error_calculation`).
 *    - Weights and biases are updated in mini-batches using backpropagation (`update_AI`).
 *    - Training progress is displayed periodically for monitoring.
 *
 * 3. **Testing / Evaluation**
 *    - The trained model is evaluated on the MNIST test dataset.
 *    - For each test sample, the predicted label is compared to the ground truth.
 *    - The final accuracy (percentage of correct guesses) is reported.
 *
 * -----------------------
 * Key design notes:
 * -----------------------
 * - Activation functions: ReLU in hidden layers, Softmax in the output layer.
 * - Loss function: Cross-entropy is implemented (`loss_function`) but not directly
 *   used in training; instead, the error calculation/backprop pipeline handles learning.
 * - Batch processing: Mini-batches are used for more stable gradient updates.
 * - Dataset: Assumes MNIST dataset is preloaded into vectors of flattened 28x28 images.
 *
 * -----------------------
 * Intended usage:
 * -----------------------
 * - Provides a minimal example of building a neural network from scratch in C++.
 * - Useful for educational purposes or extending into more complex models.
 *
 */


struct Model {
    /* 
     * A struct representing a fully-connected feedforward neural network,
     * including all parameters, intermediate values, and error terms
     * needed for training with backpropagation.
     */

    // Network architecture
    int number_of_hidden_layers;   // Number of hidden layers (not counting input/output)
    int number_of_input_nodes;     // Number of input features/nodes
    int hidden_layer_size;         // Number of neurons per hidden layer (assuming uniform size)
    vector<int> layer_sizes;       // Sizes of all layers (input, hidden(s), output)

    // Trainable parameters
    vector<vector<vector<float>>> weights; // Weights for each layer: [layer][neuron][previous_neuron]
    vector<vector<float>> bias;            // Bias terms for each layer: [layer][neuron]

    // Forward pass values
    vector<vector<float>> nuerons; // Activations of each layer: [layer][neuron]
    vector<vector<float>> z;       // Pre-activation values (weighted sums before activation): [layer][neuron]

    // Backpropagation values
    vector<vector<vector<float>>> weight_error; // Accumulated gradients for weights during backprop
    vector<vector<float>> error;                // Error terms (δ) for each layer: [layer][neuron]

    // Training hyperparameters
    float learning_rate;   // Step size for weight updates
    int batch_size;        // Number of samples per training batch
    float cost;            // Current cost (loss) for the batch

    // Labels / targets
    int label;             // True label for the current training example (used in forward/backward pass)
    int sample;
};

void loss_function(Model &AI) {
    /**
    * @brief Compute cross-entropy loss for the current sample.
    *
    * The cost is defined as:
    *     C = -∑(label[i] * log(a[i]))
    * where:
    *   - label[i] is the one-hot encoded true label
    *   - a[i] is the network's predicted probability for class i
    *
    * Since labels are one-hot vectors, this reduces to:
    *     C = -log(a[label])
    *
    * @note This function is currently not used during training,
    *       but is provided for completeness (e.g. monitoring).
    *
    * @param AI Reference to the model containing activations and label.
    */
    // Select output activation corresponding to the true label
    AI.cost = -log(AI.nuerons[AI.layer_sizes.size() - 1][AI.label]);
}

float Relu(float num){
    /*
    * Applies the Rectified Linear Unit (ReLU) activation function.
    *
    * ReLU is defined as:
    *      f(x) = max(0, x)
    *
    * It is used as the activation function for all hidden layers
    * (but not the final/output layer in this network).
    *
    * Parameters:
    *      num - The input value (typically a pre-activation value z).
    *
    * Returns:
    *      The activated value:
    *          - 0.0 if num <= 0
    *          - num otherwise
    */

    if (num <= 0 ){
        return 0.0;
    }else{
        return num;
    }

}

vector<float> Softmax(vector<float> z) {
    /*
    * Applies the Softmax activation function to a vector of values.
    *
    * Softmax is typically used in the final/output layer of a neural network
    * for multi-class classification. It converts raw scores (logits, z values)
    * into a probability distribution across all output classes.
    *
    * Formula:
    *      softmax(z_i) = exp(z_i) / Σ_j exp(z_j)
    *
    * Parameters:
    *      z - A vector of pre-activation values (logits) for the output layer.
    *
    * Returns:
    *      A vector of probabilities (same size as z), where:
    *          - Each value is between 0 and 1
    *          - All values sum to 1
    */
    float sum = 0.0f;
    vector<float> act = z;

    // Compute denominator (sum of exponentials)
    for (int i = 0; i < z.size(); i++) {
        sum += exp(z[i]);
    }
    // Compute normalized probabilities
    for (int i = 0; i < z.size(); i++) {
        act[i] = exp(z[i]) / sum;
    }

    return act;
}

float derivitive_relu(float num){
    /*
    * Computes the derivative of the Rectified Linear Unit (ReLU) function.
    *
    * ReLU is defined as:
    *      f(x) = max(0, x)
    *
    * Its derivative is:
    *      f'(x) = 0 if x <= 0
    *               1 if x > 0
    *
    * This derivative is used during backpropagation to calculate the error
    * term (δ) for neurons in hidden layers that use ReLU activation.
    *
    * Parameters:
    *      num - The pre-activation value (z) of a neuron.
    *
    * Returns:
    *      0.0 if num <= 0
    *      1.0 if num > 0
    */
    if (num <=  0.0f){
        return 0.0f ; 

    }else{
        return 1.0f;
    }
}

void activation_calculation(Model &AI, vector<float> input) {
    /*
    * Performs the forward pass through the network,
    * computing activations for every neuron in every layer.
    *
    * Process:
    *   1. Input layer activations are set directly from the input vector.
    *   2. For each hidden layer:
    *        - Compute z = W * a_prev - b
    *        - Apply ReLU activation: a = ReLU(z)
    *   3. For the final/output layer:
    *        - Compute z = W * a_prev - b
    *        - Apply Softmax activation: a = Softmax(z)
    *
    * Parameters:
    *   AI    - Reference to the Model struct containing all weights, biases,
    *           and storage for intermediate values (z and activations).
    *   input - Vector of input values (activations for the input layer).
    *
    * Updates:
    *   - AI.nuerons: stores the activations for each layer.
    *   - AI.z:       stores the pre-activation values for each layer.
    */
    // Set input layer activations
    AI.nuerons[0] = input;
    float sum;



    // Forward pass through all hidden layers (using ReLU)
    for (int i = 1; i < AI.layer_sizes.size() - 1; i++) {   // loop over hidden layers if 
        for (int j = 0; j < AI.layer_sizes[i]; j++) {       // loop over neurons in current layer
            sum = 0.0f;
            for (int k = 0; k < AI.layer_sizes[i-1]; k++) { // loop over previous layer neurons
                // Weighted sum: z = W * a_prev - b
                sum += AI.weights[i-1][k][j] * AI.nuerons[i-1][k] ;
            }
            AI.z[i][j] = sum + AI.bias[i-1][j];              // Store pre-activation
            AI.nuerons[i][j] = Relu(AI.z[i][j]);  // Apply ReLU activation

        }
    }

    // Forward pass for the final/output layer (using Softmax)
    for (int j = 0; j < AI.layer_sizes.back(); j++) {       // loop over output neurons
        sum = 0.0f;
        for (int k = 0; k < AI.layer_sizes[AI.layer_sizes.size()-2]; k++) {
            sum += AI.weights[AI.layer_sizes.size()-2][k][j] * AI.nuerons[AI.layer_sizes.size()-2][k] ;
        }
        AI.z[AI.layer_sizes.size()-1][j] = sum + AI.bias[AI.layer_sizes.size()-2][j]; // store logits
    }

    // Apply Softmax to final layer
    AI.nuerons[AI.layer_sizes.size()-1] = Softmax(AI.z[AI.layer_sizes.size()-1]);


}

void Heintialisation(vector<vector<vector<float>>> &weights) {
    /*
    * Performs He (Kaiming) initialization of the network weights.
    *
    * He initialization sets the weights of each layer based on the number
    * of input connections to that layer. This helps keep the variance of
    * activations stable across layers, especially when using ReLU activation.
    *
    * Formula:
    *      W ~ N(0, sqrt(2 / n))
    *      where:
    *          - n = number of input connections to the neuron
    *          - N = normal distribution with mean 0 and std deviation sqrt(2/n)
    *
    * Parameters:
    *      weights - A 3D vector representing the network weights:
    *                [layer][source_neuron][target_neuron]
    *
    * Notes:
    *      - Each weight is drawn independently from a normal distribution.
    *      - Using this initialization improves convergence for deep networks
    *        with ReLU activations.
    *      - Optional: seed the random engine for reproducibility.
    */
    for (int i = 0; i < weights.size(); i++) { // For each layer
        float std = sqrt(2.0f / weights[i].size()); // Standard deviation based on number of inputs

        // Random number generator for normal distribution
        default_random_engine generator;  // Can add a seed for reproducibility
        normal_distribution<double> distribution(0.0, std);

        // Initialize each weight in the layer
        for (int j = 0; j < weights[i].size(); j++) {           // For each neuron in the layer
            for (int k = 0; k < weights[i][0].size(); k++) {    // For each connection from this neuron
                weights[i][j][k] = distribution(generator);
            }
        }
    }
}

Model intialisation(vector<int> layer_sizes, string intialisation_alg) {
    /*
    * Initializes a Model struct with the specified layer sizes and weight initialization algorithm.
    *
    * This function sets up all the necessary data structures for a neural network, including:
    *   - weights
    *   - biases
    *   - activations (neurons)
    *   - pre-activations (z)
    *   - error terms
    *   - layer sizes
    *   - learning rate
    *
    * Parameters:
    *   layer_sizes        - A vector specifying the number of neurons in each layer, 
    *                        e.g., [784, 16, 16, 10].
    *   intialisation_alg  - A string specifying which weight initialization algorithm to use.
    *                        Currently supports:
    *                          - "default": uses He initialization for ReLU layers.
    *
    * Returns:
    *   A fully initialized Model struct with all vectors sized correctly and weights initialized.
    *
    * Notes:
    *   - Biases and errors are sized per layer except the input layer.
    *   - Neurons and z vectors include the input layer.
    *   - Learning rate is set to a default value of 0.01.
    */
    Model AI;

    // --- Step 1: Initialize weight matrices ---
    // weights[i] is a matrix connecting layer i to layer i+1
    vector<vector<vector<float>>> weights;
    vector<vector<vector<float>>> weights_error;
    for (int i = 0; i < layer_sizes.size() - 1; i++) {
        weights.push_back(vector<vector<float>>(layer_sizes[i],                     // rows = neurons in current layer
        vector<float>(layer_sizes[i + 1], 0.0f) // cols = neurons in next layer
        ));
    }
    weights_error = weights;
    // --- Step 2: Initialize other network vectors ---
    vector<vector<float>> bias;      // bias[i][j] = bias for neuron j in layer i+1
    vector<vector<float>> error;     // error[i][j] = accumulated error for neuron j in layer i+1
    vector<vector<float>> nuerons;   // activations per layer
    vector<vector<float>> z;         // pre-activation values per layer

    if (intialisation_alg == "default") {
        // --- Step 3: Apply weight initialization algorithm ---
        Heintialisation(weights);
        AI.weights = weights;
        AI.weight_error = weights_error;

        // --- Step 4: Initialize bias and error vectors ---
        for (int i = 0; i < layer_sizes.size() - 1; i++) { 
            bias.push_back(vector<float>(layer_sizes[i + 1], 0.0f));
            error.push_back(vector<float>(layer_sizes[i + 1], 0.0f));
        }

        // --- Step 5: Initialize neuron activations and z vectors ---
        for (int i = 0; i < layer_sizes.size(); i++) {
            nuerons.push_back(vector<float>(layer_sizes[i], 0.0f));
            z.push_back(vector<float>(layer_sizes[i], 0.0f));
        }

        // --- Step 6: Assign to Model struct ---
        AI.error = error;
        AI.bias = bias;
        AI.nuerons = nuerons;
        AI.layer_sizes = layer_sizes;
        AI.learning_rate = 0.01f; // default learning rate
        AI.z = z;
    }

    return AI;
}

vector<vector<float>> transpose_weight_matrix(vector<vector<float>> weights) {
    /*
    * Computes the transpose of a 2D weight matrix.
    *
    * Transposition swaps the rows and columns of the matrix:
    *      If the input matrix has shape [rows x cols],
    *      the result will have shape [cols x rows].
    *
    * Example:
    *      Input:
    *          [3, 4]
    *          [1, 2]
    *      Output:
    *          [3, 1]
    *          [4, 2]
    *
    * Parameters:
    *      weights - A 2D vector (matrix) of size [rows x cols],
    *                where rows = number of source neurons,
    *                and cols = number of target neurons.
    *
    * Returns:
    *      A new 2D vector representing the transposed matrix
    *      with size [cols x rows].
    *
    * Notes:
    *      - This implementation assumes all rows in `weights` 
    *        are of equal size (valid matrix).
    *      - Useful in backpropagation when reusing weight matrices
    *        to propagate error terms backward.
    */

    // Create a matrix with flipped dimensions: [cols x rows]
    vector<vector<float>> transpose(
        weights[0].size(),                   // number of rows in transpose
        vector<float>(weights.size(), 0.0f)  // number of cols in transpose
    );

    // Swap rows and columns
    for (int i = 0; i < weights.size(); i++) {
        for (int j = 0; j < weights[0].size(); j++) {
            transpose[j][i] = weights[i][j];
        }
    }

    return transpose;
}

vector<float> matrix_multiplication(vector<vector<float>> transpose, vector<float> error) {
    /*
    * Performs matrix-vector multiplication.
    *
    * Specifically computes:
    *      result = transpose * error
    *
    * Where:
    *   - transpose is a 2D matrix of size [m x n]
    *   - error is a vector of size [n]
    *   - result is a vector of size [m]
    *
    * Formula:
    *      result[i] = Σ_j (transpose[i][j] * error[j])
    *
    * Parameters:
    *      transpose - A 2D vector (matrix), often the transposed
    *                  weight matrix in backpropagation.
    *      error     - A 1D vector representing the error terms (δ)
    *                  from the next layer.
    *
    * Returns:
    *      A 1D vector of size [m], containing the propagated error
    *      for the current layer.
    *
    * Notes:
    *   - This is standard matrix-vector multiplication used in linear algebra.
    *   - In backpropagation, this is typically used to compute
    *     the error for layer L-1 given the error in layer L:
    *         δ[L-1] = (W[L]^T) * δ[L]
    */
    vector<float> result(transpose.size(), 0.0f);
    float sum;

    // For each row of the matrix
    for (int i = 0; i < transpose.size(); i++) {
        sum = 0.0f;
        // Dot product of row i with the error vector
        for (int j = 0; j < transpose[0].size(); j++) {
            sum += transpose[i][j] * error[j];
        }
        result[i] = sum;
    }

    return result;
}

void error_calculation(Model &AI) {
    /*
    * Performs backpropagation error calculation for the network.
    *
    * This function computes and accumulates the error terms (δ) used to update
    * weights and biases. The process happens per sample and is accumulated over
    * a batch before being applied during weight updates.
    *
    * Steps:
    *   1. Compute error at the output layer using the derivative of the cost
    *      function w.r.t. activations (Softmax + Cross-Entropy simplifies to y^ - y).
    *   2. Propagate error backwards through hidden layers using:
    *          δ[l] = (W[l+1]^T * δ[l+1]) ⊙ ReLU'(z[l+1])
    *      where ⊙ is element-wise multiplication.
    *   3. Compute weight error contributions:
    *          ∂C/∂W[l][j][k] = δ[l][k] * a[l][j]
    *      and accumulate them in AI.weight_error for batch updates.
    *
    * Parameters:
    *      AI - The model structure containing:
    *           - layer_sizes  : vector with size of each layer
    *           - weights      : 3D vector of weights
    *           - bias         : biases for each layer
    *           - neurons      : activations per layer
    *           - z            : pre-activation values
    *           - error        : error terms (δ) per layer
    *           - weight_error : accumulated weight gradients
    *           - label        : the true label index for the current sample
    *
    * Notes:
    *   - Errors and weight gradients are accumulated over the batch.
    *   - Only reset in update_AI() after being applied.
    *   - Output layer uses Softmax + Cross-Entropy simplification.
    */
    // --- Step 1: Output layer error calculation ---
    int output_layer_size = AI.layer_sizes.back();
    for (int i = 0; i < output_layer_size; i++) {
        if (i == AI.label) {
            // If i is the correct class: (y^ - y) = (activation - 1)
            AI.error[AI.layer_sizes.size() - 2][i] += (AI.nuerons.back()[i] - 1);
        } else {
            // If i is not the correct class: (y^ - y) = (activation - 0)
            AI.error[AI.layer_sizes.size() - 2][i] += (AI.nuerons.back()[i] - 0);
        }
    }

    // --- Step 2: Backpropagation through hidden layers ---
    vector<float> result;
    vector<vector<float>> transpose;

    // Loop backwards from second-to-last layer to first hidden layer
    for (int i = AI.layer_sizes.size() - 3; i > -1; i--) {  // should start at 1 then go to 0 since for error 0 and 1 are layer 2 and 3  

        // for default we want to loop over layers 2 and 3 which is 0 and 1 in indexes in error 
        /*
         * δ[l] = (W[l+1]^T * δ[l+1]) ⊙ ReLU'(z[l+1])
         * Note:
         *   - AI.weights[i+1] corresponds to weights between layer l+1 and l+2
         *   - Transpose ensures proper alignment for multiplication
         */

        //transpose = transpose_weight_matrix(AI.weights[i + 1]); // for first loop we need w[2]  [16[10] ]

        result = matrix_multiplication(AI.weights[i + 1], AI.error[i + 1]); // should be error 2 then 1


        // Apply ReLU derivative to propagate error
        for (int j = 0; j < AI.layer_sizes[i+1]; j++) { // [2]->16 [1]->16

            AI.error[i][j] = result[j] * derivitive_relu(AI.z[i + 1][j]);

        }
    }

    // --- Step 3: Accumulate weight error contributions ---
    for (int i = 0; i < AI.weights.size(); i++) {             // For each layer
        for (int j = 0; j < AI.weights[i].size(); j++) {      // For each neuron in layer i
            for (int k = 0; k < AI.weights[i][j].size(); k++) { // For each connection to layer i+1
                // ∂C/∂W[i][j][k] = δ[i][k] * a[i][j]
                AI.weight_error[i][j][k] += AI.error[i][k] * AI.nuerons[i][j];
            }
        }
    }

}

void update_AI(Model &AI) {
    /*
    * Updates the model parameters (weights and biases) using the
    * accumulated errors from backpropagation, then resets the error terms
    * for the next batch.
    *
    * Process:
    *   1. For each layer:
    *        - Update each weight using gradient descent:
    *              W_new = W_old - learning_rate * (dW / batch_size)
    *        - Update each bias using gradient descent:
    *              b_new = b_old - learning_rate * (db / batch_size)
    *   2. Reset error accumulators to zero in preparation for the next batch.
    *
    * Notes:
    *   - weight_error[i][j][k] should store the accumulated gradient
    *     for the weight from neuron j in layer i to neuron k in layer i+1.
    *   - error[i][j] should store the accumulated gradient for the bias
    *     of neuron j in layer i+1.
    *   - Division by batch_size ensures proper averaging across a batch.
    *
    * Parameters:
    *   AI - Reference to the Model struct containing all network parameters,
    *        gradients, and hyperparameters.
    *
    * Updates:
    *   - AI.weights: adjusted based on accumulated gradients.
    *   - AI.bias:    adjusted based on accumulated gradients.
    *   - AI.error:   reset to zero for the next batch.
    */
    int batch_size = AI.batch_size;

    // Update weights and biases for each layer
    for (int i = 0; i < AI.layer_sizes.size() - 1; i++) {          // loop over layers
        for (int j = 0; j < AI.layer_sizes[i]; j++) {              // loop over neurons in current layer
            for (int k = 0; k < AI.layer_sizes[i+1]; k++) {        // loop over neurons in next layer
                // Gradient descent update for weight
                AI.weights[i][j][k] -= AI.learning_rate * (AI.weight_error[i][j][k] / batch_size);
            }
        }
        for (int j = 0; j < AI.layer_sizes[i+1]; j++) {
            // Gradient descent update for bias
            AI.bias[i][j] -= (AI.error[i][j] / batch_size) * AI.learning_rate;
        }
    }

    // Reset error accumulators for next batch
    for (int i = 0; i < AI.layer_sizes.size() - 1; i++) {
        for (int j = 0; j < AI.layer_sizes[i+1]; j++) {
            AI.error[i][j] = 0.0f;
        }
    }
    for (int i = 0; i < AI.layer_sizes.size() - 1; i++) {          // loop over layers
        for (int j = 0; j < AI.layer_sizes[i]; j++) {              // loop over neurons in current layer
            for (int k = 0; k < AI.layer_sizes[i+1]; k++) {        // loop over neurons in next layer
                // Gradient descent update for weight
                AI.weight_error[i][j][k]  = 0.0f;
            }
        }
    }
}

int guess_label(Model &AI) {
    /*
    * Determines the predicted label of the neural network output.
    *
    * This function examines the activations of the final/output layer
    * (after Softmax) and returns the index of the neuron with the
    * highest activation. This corresponds to the network's predicted class.
    *
    * Parameters:
    *      AI - The Model struct containing the neural network, including
    *           the neuron activations for all layers.
    *
    * Returns:
    *      An integer representing the predicted class label:
    *          - 0 .. (number of output neurons - 1)
    *          - -1 if no neuron has a value greater than the initial max (rare case)
    *
    * Notes:
    *      - Assumes the output layer activations are probabilities from Softmax.
    *      - Uses a simple argmax to select the predicted class.
    */
    float max = 0.0f;      // Track the highest activation value
    int guess = -1;         // Track the index of the neuron with highest activation

    // Loop over all neurons in the output layer
    for (int i = 0; i < AI.layer_sizes.back(); i++) {
        //cout<<AI.nuerons[AI.layer_sizes.size() - 1][i]<<" \n";

        if (AI.nuerons[AI.layer_sizes.size() - 1][i] > max) {
            max = AI.nuerons[AI.layer_sizes.size() - 1][i]; // Update max value
            guess = i;                                     // Update predicted class index
        }
    }

    return guess;
}






void trainAI() {
    /*
    * Train and evaluate a simple feedforward neural network on MNIST.
    *
    * This function initializes the network, loads the dataset, runs training
    * with mini-batch gradient descent, and finally evaluates accuracy on the test set.
    */

    ////////////////////////////////////////////////////
    // Step 1: Network Initialization
    ////////////////////////////////////////////////////

    int input_layer_size = 784;           // Input: 28x28 pixel images flattened
    vector<int> hidden_layer_size(2, 16); // Hidden architecture: two layers, 16 neurons each
    int output_layer_size = 10;           // Output: 10 classes (digits 0–9)

    // Construct full layer size vector [input -> hidden layers -> output]
    vector<int> layer_size = {input_layer_size};
    for (int i = 0; i < hidden_layer_size.size(); i++) {
        layer_size.push_back(hidden_layer_size[i]);
    }
    layer_size.push_back(output_layer_size);

    // Load dataset (training + testing data + labels)
    Dataset ds = data_setup();

    // Initialize model parameters (weights, biases) with He initialization
    Model AI = intialisation(layer_size, "default");
    AI.batch_size = 20; // Gradient updates applied after every 20 samples

    int train_dataset_size = ds.train_data.size() / 784; // Number of training images
    vector<float> input(input_layer_size, 0.0f);         // Reusable input buffer

    AI.sample = 0;      // Current sample index
    int count = 0;      // Progress bar tracker
    float prev_loss = 100; // Placeholder for tracking convergence (not yet used)

    ////////////////////////////////////////////////////
    // Step 2: Training Loop
    ////////////////////////////////////////////////////
    for (int i = 0; i < train_dataset_size; i++) {

        // Load current sample label
        AI.label = ds.train_labels[AI.sample];

        // Flattened image → input vector
        for (int j = 0; j < input_layer_size; j++) {
            input[j] = ds.train_data[j + (AI.sample * input_layer_size)];
        }

        // Forward pass
        activation_calculation(AI, input);

        // Backpropagation (compute errors/gradients)
        error_calculation(AI);

        // Apply gradient update once batch is complete
        if (AI.sample % AI.batch_size == 0) {
            update_AI(AI);
        }

        // Print simple progress indicator every ~10k samples
        if (AI.sample % 9999 == 0) {
            cout << "training[";
            for (int i = 0; i < count; i++) cout << "*";
            for (int i = 0; i < 6 - count; i++) cout << " ";
            cout << "]\n";
            count++;
        }

        AI.sample++;
    }

    ////////////////////////////////////////////////////
    // Step 3: Testing / Evaluation
    ////////////////////////////////////////////////////
    int test_dataset_size = ds.test_data.size() / 784;
    int sample = 0;
    int guess = 0;
    int number_of_correct_guesses = 0;

    for (int i = 0; i < test_dataset_size; i++) {
        // Load current test label
        AI.label = ds.test_labels[sample];

        // Flattened image → input vector
        for (int j = 0; j < input_layer_size; j++) {
            input[j] = ds.test_data[j + (sample * input_layer_size)];
        }

        // Forward pass
        activation_calculation(AI, input);

        // Predict class (argmax of output layer)
        guess = guess_label(AI);

        // Track accuracy
        if (guess == AI.label) {
            number_of_correct_guesses++;
        } 

        sample++;
    }

    // Report overall test accuracy
    float percentage_of_correct_guesses = static_cast<float>(number_of_correct_guesses) / test_dataset_size;

    cout << "Percentage of correct guesses: " << percentage_of_correct_guesses << "\n";
}
