// HW1

#include <iostream>
#include <list>
#include <cstdlib>
#include <math.h>
using namespace std;



double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }
double init_weight() { return ((double)rand()) / ((double)RAND_MAX); } // initial weight to 0~1
void init_double_pointer(double **ia, int sizex, int sizey)
{
    for (int y = 0; y < sizex; y++)
        *(ia + y) = (double *)malloc(sizey * sizeof(double *));
}

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = *(array + j);
            *(array + j) = *(array + i);
            *(array + i) = t;
        }
    }
}
void predict(double *hiddenLayerBias, double **hiddenWeights, double *outputLayerBias, double *hiddenLayer, double **outputWeights, double *outputLayer);

int main(int argc, const char *argv[])
{
    
    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;

    const double lr = 0.1f;

    double *hiddenLayer = (double *)malloc(numHiddenNodes * sizeof(double));
    double *outputLayer = (double *)malloc(numOutputs * sizeof(double));

    double *hiddenLayerBias = (double *)malloc(numHiddenNodes * sizeof(double));
    double *outputLayerBias = (double *)malloc(numOutputs * sizeof(double));

    double **hiddenWeights = (double **)malloc(numInputs * sizeof(double));
    init_double_pointer(hiddenWeights, numInputs, numHiddenNodes);
    double **outputWeights = (double **)malloc(numHiddenNodes * sizeof(double));
    init_double_pointer(outputWeights, numHiddenNodes, numOutputs);

    static const int numTrainingSets = 4;
    double **training_inputs = (double **)malloc(numTrainingSets * sizeof(double));
    init_double_pointer(training_inputs, numTrainingSets, numInputs);
    double **training_outputs = (double **)malloc(numTrainingSets * sizeof(double));
    init_double_pointer(training_outputs, numTrainingSets, numOutputs);

    // Initialization
    for (int i = 0; i < numInputs; i++)
    {
        for (int j = 0; j < numHiddenNodes; j++)
        {
            *(*(hiddenWeights + i) + j) = init_weight();
        }
    }
    for (int i = 0; i < numHiddenNodes; i++)
    {
        *(hiddenLayerBias + i) = init_weight();
        for (int j = 0; j < numOutputs; j++)
        {
            *(*(outputWeights + i) + j) = init_weight();
        }
    }
    for (int i = 0; i < numOutputs; i++)
    {
        *(outputLayerBias + i) = init_weight();
    }
    {
        *(*(training_inputs + 0) + 0) = 0;
        *(*(training_inputs + 0) + 1) = 0;
        *(*(training_inputs + 1) + 0) = 0;
        *(*(training_inputs + 1) + 1) = 1;
        *(*(training_inputs + 2) + 0) = 1;
        *(*(training_inputs + 2) + 1) = 0;
        *(*(training_inputs + 3) + 0) = 1;
        *(*(training_inputs + 3) + 1) = 1;

        *(*(training_outputs + 0)) = 0;
        *(*(training_outputs + 1)) = 1;
        *(*(training_outputs + 2)) = 1;
        *(*(training_outputs + 3)) = 0;
    }

    static const int numtrainingSet = 4;
    int *trainingSetOrder = (int *)malloc(numTrainingSets * sizeof(int));
    for (int i = 0; i < numTrainingSets; i++)
    {
        *(trainingSetOrder + i) = i;
    }

    // Training

    static const int epochs = 10000;
    double loss;
    for (int n = 0; n < epochs; n++)
    {
        shuffle(trainingSetOrder, numTrainingSets);
        for (int x = 0; x < numTrainingSets; x++)
        {

            int i = *(trainingSetOrder + x);

            // Forward pass

            for (int j = 0; j < numHiddenNodes; j++)
            {
                double activation = *(hiddenLayerBias + j);
                for (int k = 0; k < numInputs; k++)
                {
                    activation += *(*(training_inputs + i) + k) * *(*(hiddenWeights + k) + j);
                }
                *(hiddenLayer + j) = sigmoid(activation);
            }

            for (int j = 0; j < numOutputs; j++)
            {
                double activation = *(outputLayerBias + j);
                for (int k = 0; k < numHiddenNodes; k++)
                {
                    activation += *(hiddenLayer + k) * *(*(outputWeights + k) + j);
                }
                *(outputLayer + j) = sigmoid(activation);
            }

            // printf("Input: %.0f %.0f    Output: %f     Expected Output: %.0f \n", *(*(training_inputs + i) + 0), *(*(training_inputs + i) + 1), *(outputLayer + 0), *(*(training_outputs + i) + 0));

            // Backprop

            double *deltaOutput = (double *)malloc(numOutputs * sizeof(double));

            for (int j = 0; j < numOutputs; j++)
            {
                double errorOutput = (*(*(training_outputs + i) + j) - *(outputLayer + j));
                loss = (errorOutput * errorOutput) / 2;
                *(deltaOutput + j) = errorOutput * dSigmoid(*(outputLayer + j));
            }

            double *deltaHidden = (double *)malloc(numHiddenNodes * sizeof(double));
            for (int j = 0; j < numHiddenNodes; j++)
            {
                double errorHidden = 0.0f;
                for (int k = 0; k < numOutputs; k++)
                {
                    errorHidden += *(deltaOutput + k) * *(*(outputWeights + j) + k);
                }
                *(deltaHidden + j) = errorHidden * dSigmoid(*(hiddenLayer + j));
            }

            for (int j = 0; j < numOutputs; j++)
            {
                *(outputLayerBias + j) += *(deltaOutput + j) * lr;
                for (int k = 0; k < numHiddenNodes; k++)
                {
                    *(*(outputWeights + k) + j) += *(hiddenLayer + k) * *(deltaOutput + j) * lr;
                }
            }

            for (int j = 0; j < numHiddenNodes; j++)
            {
                *(hiddenLayerBias + j) += *(deltaHidden + j) * lr;
                for (int k = 0; k < numInputs; k++)
                {
                    *(*(hiddenWeights + k) + j) += *(*(training_inputs + i) + k) * *(deltaHidden + j) * lr;
                }
            }
        }
        // printf("Epoch: %d  Loss: %f \n", n, loss);
    }

    // Print weights
    printf("Final Hidden Weights\n[");
    for (int j = 0; j < numHiddenNodes; j++)
    {
        printf("[ ");
        for (int k = 0; k < numInputs; k++)
        {
            printf("%f ", *(*(hiddenWeights + k) + j));
        }
        printf("] ");
    }
    printf("]\n");

    printf("Final Hidden Biases\n[ ");
    for (int j = 0; j < numHiddenNodes; j++)
    {
        printf("%f ", *(hiddenLayerBias + j));
    }
    printf("]\n");
    printf("Final Output Weights\n");
    for (int j = 0; j < numOutputs; j++)
    {
        printf("[ ");
        for (int k = 0; k < numHiddenNodes; k++)
        {
            printf("%f ", *(*(outputWeights + k) + j));
        }
        printf("]\n");
    }
    printf("Final Output Biases\n[ ");
    for (int j = 0; j < numOutputs; j++)
    {
        printf("%f ", *(outputLayerBias + j));
    }
    printf("]\n");

    predict(hiddenLayerBias, hiddenWeights, outputLayerBias, hiddenLayer, outputWeights, outputLayer);

    return 0;
}

void predict(double *hiddenLayerBias, double **hiddenWeights, double *outputLayerBias, double *hiddenLayer, double **outputWeights, double *outputLayer)
{
    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;

    // Input
    int *in = (int *)malloc(numInputs * sizeof(int));
    int in1, in2;
    printf("Please enter the first bit: \n");
    scanf("%d", &in1);
    printf("Please enter the second bit: \n");
    scanf("%d", &in2);
    *in = in1;
    *(in + 1) = in2;
    // printf("in1:%d in2:%d \n", *in, *(in + 1));

    for (int j = 0; j < numHiddenNodes; j++)
    {
        double activation = *(hiddenLayerBias + j);
        for (int k = 0; k < numInputs; k++)
        {
            activation += *(in + k) * *(*(hiddenWeights + k) + j);
        }
        *(hiddenLayer + j) = sigmoid(activation);
    }

    for (int j = 0; j < numOutputs; j++)
    {
        double activation = *(outputLayerBias + j);
        for (int k = 0; k < numHiddenNodes; k++)
        {
            activation += *(hiddenLayer + k) * *(*(outputWeights + k) + j);
        }
        *(outputLayer + j) = sigmoid(activation);
    }

    printf("output:%f", *outputLayer);
}