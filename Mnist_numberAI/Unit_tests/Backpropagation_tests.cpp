#include "../AI_alg.cpp"





void Relu_tests(){
    float a = -1.2, b= 1.5, ansa=0 , ansb =1.5 ;
    float c=-1.2 , d=1.5, ansc=0 , ansd=1.0 ;
    a = Relu(a);
    b = Relu(b);
    c = derivitive_relu(c);
    d = derivitive_relu(d);

    int passtest=0;
    if (a == ansa){
        passtest++;
    }else{
        cout<<"Relu test 1 failed"<< "\n";
    }
    if (b == ansb){
        passtest++;
    }else{
        cout<<"Relu test 2 failed"<< "\n";
    }

    if (c == ansc){
        passtest++;
    }else{
        cout<<"Relu test 1 failed"<< "\n";
    }
    if (d == ansd){
        passtest++;
    }else{
        cout<<"Relu test 2 failed"<< "\n";
    }

    if (passtest == 4){
        cout<<"relu tests passed"<< "\n";
    }
}
bool vectors_equal(const vector<float>& a, const vector<float>& b, float tol = 1e-5) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (abs(a[i] - b[i]) > tol) return false;
    }
    return true;
}
bool matrices_equal(const vector<vector<float>>& a, const vector<vector<float>>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].size() != b[i].size()) return false;
        for (size_t j = 0; j < a[i].size(); j++) {
            if (abs(a[i][j] - b[i][j]) > 1e-5) return false; // tolerance
        }
    }
    return true;
}

bool vectors_are_close(const vector<float>& v1, const vector<float>& v2, float tol = 1e-4) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); i++) {
        if (fabs(v1[i] - v2[i]) > tol) {
            return false;
        }
    }
    return true;
}

void Softmax_tests() {
    int test_passed = 0;

    vector<float> a = {1.0, 2.0, 3.0}; 
    vector<float> ansa = {0.0900, 0.2447, 0.6652};
    vector<float> b = {5.0, 5.0, 5.0};
    vector<float> ansb = {0.3333, 0.3333, 0.3333};
    vector<float> c = {-1.0, -2.0, -3.0};
    vector<float> ansc = {0.6652, 0.2447, 0.0900};

    a = Softmax(a);  // your function should return a vector<float>
    b = Softmax(b);
    c = Softmax(c);
    if (vectors_are_close(a, ansa)) {
        test_passed++;
        //cout << "Softmax test 1 passed!" << endl;
    } else {
        cout << "Softmax test 1 failed!" << endl;
        cout << "Got: ";
        for (float val : a) cout << val << " ";
        cout << "\nExpected: ";
        for (float val : ansa) cout << val << " ";
        cout << endl;
    }
    if (vectors_are_close(b, ansb)) {
        test_passed++;
        //cout << "Softmax test 1 passed!" << endl;
    } else {
        cout << "Softmax test 2 failed!" << endl;
        cout << "Got: ";
        for (float val : b) cout << val << " ";
        cout << "\nExpected: ";
        for (float val : ansb) cout << val << " ";
        cout << endl;
    }
        if (vectors_are_close(c, ansc)) {
        test_passed++;
        //cout << "Softmax test 1 passed!" << endl;
    } else {
        cout << "Softmax test 3 failed!" << endl;
        cout << "Got: ";
        for (float val : c) cout << val << " ";
        cout << "\nExpected: ";
        for (float val : ansc) cout << val << " ";
        cout << endl;
    }
    if(test_passed == 3){
        cout<<"softmax tests passed"<< "\n";
    }
}

void transpose_weight_matrix_tests(){

    int tests_passed = 0;

    // Test 1: Square Matrix
    vector<vector<float>> m1 = {{1, 2}, {3, 4}};
    vector<vector<float>> ans1 = {{1, 3}, {2, 4}};
    if (matrices_equal(transpose_weight_matrix(m1), ans1)) tests_passed++;
    else cout << "Transpose test 1 failed\n";

    // Test 2: Rectangular (2x3)
    vector<vector<float>> m2 = {{1, 2, 3}, {4, 5, 6}};
    vector<vector<float>> ans2 = {{1, 4}, {2, 5}, {3, 6}};
    if (matrices_equal(transpose_weight_matrix(m2), ans2)) tests_passed++;
    else cout << "Transpose test 2 failed\n";

    // Test 3: Rectangular (3x2)
    vector<vector<float>> m3 = {{1, 2}, {3, 4}, {5, 6}};
    vector<vector<float>> ans3 = {{1, 3, 5}, {2, 4, 6}};
    if (matrices_equal(transpose_weight_matrix(m3), ans3)) tests_passed++;
    else cout << "Transpose test 3 failed\n";

    // Test 4: Single Row
    vector<vector<float>> m4 = {{7, 8, 9}};
    vector<vector<float>> ans4 = {{7}, {8}, {9}};
    if (matrices_equal(transpose_weight_matrix(m4), ans4)) tests_passed++;
    else cout << "Transpose test 4 failed\n";

    // Test 5: Single Column
    vector<vector<float>> m5 = {{1}, {2}, {3}};
    vector<vector<float>> ans5 = {{1, 2, 3}};
    if (matrices_equal(transpose_weight_matrix(m5), ans5)) tests_passed++;
    else cout << "Transpose test 5 failed\n";

    // Test 6: 1x1 Matrix
    vector<vector<float>> m6 = {{42}};
    vector<vector<float>> ans6 = {{42}};
    if (matrices_equal(transpose_weight_matrix(m6), ans6)) tests_passed++;
    else cout << "Transpose test 6 failed\n";

    cout << "transpose tests passed.\n";
}

void matrix_multiplication_tests(){
     int tests_passed = 0;

    // Test 1: 2x2 identity matrix
    vector<vector<float>> m1 = {{1, 0}, {0, 1}};
    vector<float> v1 = {5, 7};
    vector<float> ans1 = {5, 7};
    if (vectors_equal(matrix_multiplication(m1, v1), ans1)) tests_passed++;
    else cout << "MatVecMul test 1 failed\n";

    // Test 2: 2x2 scaling matrix
    vector<vector<float>> m2 = {{2, 0}, {0, 3}};
    vector<float> v2 = {4, 5};
    vector<float> ans2 = {8, 15};
    if (vectors_equal(matrix_multiplication(m2, v2), ans2)) tests_passed++;
    else cout << "MatVecMul test 2 failed\n";

    // Test 3: 3x2 matrix
    vector<vector<float>> m3 = {{1, 2}, {3, 4}, {5, 6}};
    vector<float> v3 = {1, 1};
    vector<float> ans3 = {3, 7, 11};
    if (vectors_equal(matrix_multiplication(m3, v3), ans3)) tests_passed++;
    else cout << "MatVecMul test 3 failed\n";

    // Test 4: 2x3 matrix
    vector<vector<float>> m4 = {{1, 0, -1}, {2, 3, 4}};
    vector<float> v4 = {1, 2, 3};
    vector<float> ans4 = {-2, 20};
    if (vectors_equal(matrix_multiplication(m4, v4), ans4)) tests_passed++;
    else cout << "MatVecMul test 4 failed\n";

    // Test 5: Zero vector
    vector<vector<float>> m5 = {{7, 8}, {9, 10}};
    vector<float> v5 = {0, 0};
    vector<float> ans5 = {0, 0};
    if (vectors_equal(matrix_multiplication(m5, v5), ans5)) tests_passed++;
    else cout << "MatVecMul test 5 failed\n";

    cout << "vector multiplication tests passed.\n";
}


void secondary_function_test(){
    Relu_tests();
    Softmax_tests();
    transpose_weight_matrix_tests();
    matrix_multiplication_tests();

}
bool not_all_zero(const vector<vector<vector<float>>> &weights) {
    for (const auto &layer : weights)
        for (const auto &neuron : layer)
            for (float w : neuron)
                if (fabs(w) > 1e-6) return true;
    return false;
}
void Heintialisation_tests(){
    int tests_passed = 0;

    // Test 1: Simple 1-layer network, 2 neurons, 3 inputs each
    {
        vector<vector<vector<float>>> weights(1, vector<vector<float>>(2, vector<float>(3, 0)));
        Heintialisation(weights);

        if (weights[0].size() == 2 && weights[0][0].size() == 3 && not_all_zero(weights)) {
            tests_passed++;
        } else {
            cout << "HeInitialisation test 1 failed\n";
        }
    }

    // Test 2: 2 layers, check dimensions preserved
    {
        vector<vector<vector<float>>> weights(2);
        weights[0] = vector<vector<float>>(3, vector<float>(4, 0)); // layer 1: 3 neurons, 4 inputs
        weights[1] = vector<vector<float>>(2, vector<float>(3, 0)); // layer 2: 2 neurons, 3 inputs
        Heintialisation(weights);

        if (weights[0].size() == 3 && weights[0][0].size() == 4 &&
            weights[1].size() == 2 && weights[1][0].size() == 3 &&
            not_all_zero(weights)) {
            tests_passed++;
        } else {
            cout << "HeInitialisation test 2 failed\n";
        }
    }

    // Test 3: Range check (values not exploding)
    {
        vector<vector<vector<float>>> weights(1, vector<vector<float>>(5, vector<float>(10, 0)));
        Heintialisation(weights);

        bool in_range = true;
        for (auto &neuron : weights[0]) {
            for (float w : neuron) {
                if (fabs(w) > 5.0f) { // arbitrary sanity check
                    in_range = false;
                    break;
                }
            }
        }
        if (in_range) tests_passed++;
        else cout << "HeInitialisation test 3 failed\n";
    }

    cout << tests_passed << "/3 He initialisation tests passed.\n";
}

void Initialization_test(){
    int tests_passed = 0;
    vector<int> layers = {4, 3, 2};
    Model AI = intialisation(layers, "default");

    // --- Check layer sizes ---
    if (AI.layer_sizes == layers) tests_passed++;
    else cout << "Test 1a failed: Layer sizes incorrect\n";

    // --- Check weights dimensions ---
    bool weights_ok = AI.weights.size() == layers.size() - 1 &&
                      AI.weights[0].size() == layers[0] &&
                      AI.weights[0][0].size() == layers[1] &&
                      AI.weights[1].size() == layers[1] &&
                      AI.weights[1][0].size() == layers[2];
    if (weights_ok) tests_passed++;
    else cout << "Test 1b failed: Weights dimensions incorrect\n";

    // --- Check bias and error sizes ---
    bool bias_error_ok = AI.bias.size() == layers.size() - 1 &&
                         AI.bias[0].size() == layers[1] &&
                         AI.bias[1].size() == layers[2] &&
                         AI.error.size() == layers.size() - 1 &&
                         AI.error[0].size() == layers[1] &&
                         AI.error[1].size() == layers[2];
    if (bias_error_ok) tests_passed++;
    else cout << "Test 1c failed: Bias or error dimensions incorrect\n";

    // --- Check neuron and z sizes ---
    bool neurons_ok = AI.nuerons.size() == layers.size() &&
                      AI.nuerons[0].size() == layers[0] &&
                      AI.nuerons[1].size() == layers[1] &&
                      AI.nuerons[2].size() == layers[2] &&
                      AI.z.size() == layers.size();
    if (neurons_ok) tests_passed++;
    else cout << "Test 1d failed: Neurons or z dimensions incorrect\n";

    // --- Check initialization values ---
    bool zeros_ok = true;
    for (auto &b_layer : AI.bias)
        for (float b : b_layer) if (b != 0.0f) zeros_ok = false;
    for (auto &e_layer : AI.error)
        for (float e : e_layer) if (e != 0.0f) zeros_ok = false;
    for (auto &n_layer : AI.nuerons)
        for (float n : n_layer) if (n != 0.0f) zeros_ok = false;
    for (auto &z_layer : AI.z)
        for (float z_val : z_layer) if (z_val != 0.0f) zeros_ok = false;

    if (zeros_ok) tests_passed++;
    else cout << "Test 1e failed: Bias, error, neurons, or z not initialized to 0\n";

    // --- Check learning rate ---
    if (AI.learning_rate == 0.01f) tests_passed++;
    else cout << "Test 1f failed: Learning rate incorrect\n";

    cout << tests_passed << "/6 Model initialization tests passed.\n";
}

bool approx_equal(float a, float b, float tol = 1e-5) {
    return fabs(a - b) < tol;
}

void ActivationCalculation_tests() {
    int tests_passed = 0;

    // --- Setup a small deterministic network ---
    Model AI;
    AI.layer_sizes = {3, 2, 2}; // 3 inputs, 2 hidden neurons, 2 output neurons
    AI.nuerons = vector<vector<float>>(3);
    AI.z = vector<vector<float>>(3);
    AI.weights = {
        {{0.1f, 0.2f}, {0.3f, 0.4f}}, // input -> hidden
        {{0.5f, 0.6f}, {0.7f, 0.8f}}  // hidden -> output
    };
    AI.bias = {
        {0.1f, 0.1f}, // hidden layer bias
        {0.0f, 0.0f}  // output layer bias
    };

    for (int i = 0; i < 3; i++) {
        AI.nuerons[i] = vector<float>(AI.layer_sizes[i], 0.0f);
        AI.z[i] = vector<float>(AI.layer_sizes[i], 0.0f);
    }

    vector<float> input = {1.0f, 2.0f, 3.0f};
    

    // --- Run activation calculation ---
    activation_calculation(AI, input);
    
    // --- Test 1: Input layer activations equal input ---
    bool input_ok = true;
    for (int i = 0; i < 3; i++) {
        if (!approx_equal(AI.nuerons[0][i], input[i])) input_ok = false;
    }
    if (input_ok) tests_passed++;
    else cout << "Activation test 1 failed: input layer mismatch\n";

    // --- Test 2: Hidden layer activations non-negative (ReLU) ---
    bool hidden_ok = true;
    for (float val : AI.nuerons[1]) if (val < 0.0f) hidden_ok = false;
    if (hidden_ok) tests_passed++;
    else cout << "Activation test 2 failed: hidden layer negative value\n";

    // --- Test 3: Output layer sum = 1 (Softmax property) ---
    float sum_output = 0.0f;
    for (float val : AI.nuerons[2]) sum_output += val;
    if (approx_equal(sum_output, 1.0f)) tests_passed++;
    else cout << "Activation test 3 failed: Softmax output sum != 1\n";

    // --- Test 4: Shapes match layer_sizes ---
    bool shape_ok = (AI.nuerons.size() == AI.layer_sizes.size()) &&
                    (AI.z.size() == AI.layer_sizes.size()) &&
                    (AI.nuerons[1].size() == 2) &&
                    (AI.nuerons[2].size() == 2);
    if (shape_ok) tests_passed++;
    else cout << "Activation test 4 failed: neuron/z shape mismatch\n";

    cout << tests_passed << "/4 activation calculation tests passed.\n";
}

void error_calculation_test(){
    int tests_passed = 0;

    // --- Setup a tiny deterministic network ---
    Model AI;
    AI.layer_sizes = {3, 2, 2}; // 3 inputs, 2 hidden neurons, 2 output neurons
    AI.nuerons = vector<vector<float>>(3);
    AI.z = vector<vector<float>>(3);
    AI.weights = {
        {{0.1f, 0.2f}, {0.3f, 0.4f}}, // input -> hidden
        {{0.5f, 0.6f}, {0.7f, 0.8f}}  // hidden -> output
    };
    AI.bias = {
        {0.0f, 0.0f}, // hidden
        {0.0f, 0.0f}  // output
    };
    AI.error = {
        vector<float>(2, 0.0f),
        vector<float>(2, 0.0f)
    };
    AI.z[0] = {1.0f, 2.0f, 3.0f}; // input layer
    AI.z[1] = {1.0f, 2.0f};       // hidden layer (ReLU outputs)
    AI.z[2] = {0.6f, 0.4f}; 

    AI.weight_error = AI.weights;
    for (auto &layer : AI.weight_error)
        for (auto &neuron : layer)
            for (auto &w : neuron) w = 0.0f;

    // Set activations manually (after a forward pass)
    AI.nuerons[0] = {1.0f, 2.0f, 3.0f}; // input layer
    AI.nuerons[1] = {1.0f, 2.0f};       // hidden layer (ReLU outputs)
    AI.nuerons[2] = {0.6f, 0.4f};       // output layer (Softmax outputs)

    AI.label = 0; // correct class is first output neuron

    // --- Run error calculation ---
    error_calculation(AI);

    // --- Test 1: Output layer error = a - y ---
    bool output_ok = approx_equal(AI.error[1][0], AI.nuerons[2][0] - 1.0f) &&
                     approx_equal(AI.error[1][1], AI.nuerons[2][1] - 0.0f);
    if (output_ok) tests_passed++;
    else cout << "Error test 1 failed: output layer error incorrect\n";

    // --- Test 2: Weight error accumulation correctness for output layer ---
    bool weight_error_ok = approx_equal(AI.weight_error[1][0][0], AI.error[1][0] * AI.nuerons[1][0]) &&
                           approx_equal(AI.weight_error[1][1][1], AI.error[1][1] * AI.nuerons[1][1]);
    if (weight_error_ok) tests_passed++;
    else cout << "Error test 2 failed: weight_error incorrect\n";

    // --- Test 3: Hidden layer errors are non-negative due to ReLU derivative ---
    bool hidden_ok = true;
    for (float val : AI.error[0]) if (val < 0.0f) hidden_ok = false;
    if (hidden_ok) tests_passed++;
    else cout << "Error test 3 failed: hidden layer negative error\n";

    // --- Test 4: Error shapes match layers ---
    bool shape_ok = (AI.error.size() == AI.layer_sizes.size() - 1) &&
                    (AI.error[0].size() == AI.layer_sizes[1]) &&
                    (AI.error[1].size() == AI.layer_sizes[2]);
    if (shape_ok) tests_passed++;
    else cout << "Error test 4 failed: error shapes incorrect\n";

    cout << tests_passed << "/4 error calculation tests passed.\n";
}


void DeterministicErrorCalculation_test() {
    int tests_passed = 0;

    // --- Setup tiny deterministic network ---
    Model AI;
    AI.layer_sizes = {2, 2, 2}; // input 2, hidden 2, output 2
    AI.nuerons = vector<vector<float>>(3);
    AI.z = vector<vector<float>>(3);
    AI.weights = {
        {{0.1f, 0.2f}, {0.3f, 0.4f}}, // input -> hidden
        {{0.5f, 0.6f}, {0.7f, 0.8f}}  // hidden -> output
    };
    AI.bias = {
        {0.0f, 0.0f}, // hidden
        {0.0f, 0.0f}  // output
    };
    AI.error = {
        vector<float>(2, 0.0f),
        vector<float>(2, 0.0f)
    };
    AI.weight_error = {
        {{0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}}
    };
    AI.z[0] = {1.0f, 2.0f, 3.0f}; // input layer
    AI.z[1] = {1.0f, 2.0f};       // hidden layer (ReLU outputs)
    AI.z[2] = {0.6f, 0.4f}; 

    // --- Fixed activations (after forward pass) ---
    AI.nuerons[0] = {1.0f, 2.0f}; // input
    AI.nuerons[1] = {0.8f, 1.2f}; // hidden layer after ReLU
    AI.nuerons[2] = {0.6f, 0.4f}; // output after Softmax

    AI.label = 0; // correct class is first output neuron

    // --- Run error calculation ---
    error_calculation(AI);

    // --- Manual expected values ---
    // Output layer error = a - y
    vector<float> expected_output_error = {0.6f - 1.0f, 0.4f - 0.0f}; // {-0.4, 0.4}

    // Hidden layer error: δ_hidden = W_output^T * δ_output ⊙ ReLU'(z_hidden)
    // ReLU derivative = 1 since z_hidden > 0
    // W_output^T = [[0.5, 0.7],[0.6,0.8]]
    // δ_output = {-0.4, 0.4}
    // δ_hidden[0] = 0.5*-0.4 + 0.7*0.4 = -0.2 + 0.28 = 0.08
    // δ_hidden[1] = 0.6*-0.4 + 0.8*0.4 = -0.24 + 0.32 = 0.08
    vector<float> expected_hidden_error = {0.08f, 0.08f};

    // --- Test output layer error ---
    bool output_ok = approx_equal(AI.error[1][0], expected_output_error[0]) &&
                     approx_equal(AI.error[1][1], expected_output_error[1]);
    if (output_ok) tests_passed++;
    else cout << "Deterministic Test 1 failed: output layer error incorrect\n";

    // --- Test hidden layer error ---
    bool hidden_ok = approx_equal(AI.error[0][0], expected_hidden_error[0]) &&
                     approx_equal(AI.error[0][1], expected_hidden_error[1]);
    if (hidden_ok) tests_passed++;
    else {
        cout << "Deterministic Test 2 failed: hidden layer error incorrect\n";
        cout<< "we got : "<<AI.error[0][0]<< "expected : "<< expected_hidden_error[0]<<"we got : "<<AI.error[0][1]<< "\nexpected : "<< expected_hidden_error[1]<<"\n";
    }
    // --- Test weight error accumulation for output layer ---
    // weight_error[i][j][k] = δ_output[k] * a_hidden[j]
    bool weight_ok = approx_equal(AI.weight_error[1][0][0], expected_output_error[0] * AI.nuerons[1][0]) &&
                     approx_equal(AI.weight_error[1][1][1], expected_output_error[1] * AI.nuerons[1][1]);
    if (weight_ok) tests_passed++;
    else cout << "Deterministic Test 3 failed: output layer weight error incorrect\n";

    cout << tests_passed << "/3 deterministic error calculation tests passed.\n";
}



void AI_tests(){
    Heintialisation_tests();
    Initialization_test();
    ActivationCalculation_tests(); // still need to check outputs but sizing is correct
    error_calculation_test(); // might need to do more testing 
    DeterministicErrorCalculation_test();
}


int main (){
    secondary_function_test();
    AI_tests();
    return 0;
}