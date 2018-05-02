#include "Matrix.h"

struct NeuralNetwork {
	int input;
	int hidden;
	int output;
	float learning_rate;
	struct Matrix ih_weights;
	struct Matrix ho_weights;
	struct Matrix h_bias;
	struct Matrix o_bias;
} NeuralNetwork;

int initNeuralNetwork(struct NeuralNetwork *nn, int input, int hidden, int output, float lrate){

	nn->ih_weights.rows = hidden;
	nn->ih_weights.cols = input;
	allocMatrix(&nn->ih_weights);
	randomizeMatrix(&nn->ih_weights);

	nn->ho_weights.rows = output;
	nn->ho_weights.cols = hidden;
	allocMatrix(&nn->ho_weights);
	randomizeMatrix(&nn->ho_weights);

	nn->h_bias.rows = hidden;
	nn->h_bias.cols = 1;
	allocMatrix(&nn->h_bias);
	randomizeMatrix(&nn->h_bias);

	nn->o_bias.rows = output;
	nn->o_bias.cols = 1;
	allocMatrix(&nn->o_bias);
	randomizeMatrix(&nn->o_bias);

	nn->input = input;
	nn->hidden = hidden;
	nn->output = output;
	nn->learning_rate = lrate;

	return 0;
}

int deallocNNMatricies(struct NeuralNetwork *nn) {
	deallocMatrix(&nn->ih_weights);
	deallocMatrix(&nn->ho_weights);
	deallocMatrix(&nn->h_bias);
	deallocMatrix(&nn->o_bias);
	return 0;
}

int feedforward(struct NeuralNetwork *nn, float input[], float result[]){
	struct Matrix inputs;
	struct Matrix hidden;
	struct Matrix outputs;

	inputFromArray(input, nn->input, &inputs);
	product(&nn->ih_weights, &inputs, &hidden);
	matrixAdd(&hidden, &nn->h_bias);
	sigmoid(&hidden);

	product(&nn->ho_weights, &hidden, &outputs);
	matrixAdd(&outputs, &nn->o_bias);
	sigmoid(&outputs);

	matrixToArray(&outputs, result);

	deallocMatrix(&inputs);
	deallocMatrix(&hidden);
	deallocMatrix(&outputs);

	return 0;
}

int train(struct NeuralNetwork *nn, float input[], float target[]){
	struct Matrix inputs;
	struct Matrix hidden;
	struct Matrix outputs;
	struct Matrix targets;
	struct Matrix output_errors;
	struct Matrix gradients;
	struct Matrix hidden_T;
	struct Matrix weight_ho_deltas;
	struct Matrix who_t;
	struct Matrix hidden_errors;
	struct Matrix hidden_gradient;
	struct Matrix inputs_T;
	struct Matrix weight_ih_deltas;

	inputFromArray(input, nn->input, &inputs);

	product(&nn->ih_weights, &inputs, &hidden);
	matrixAdd(&hidden, &nn->h_bias);
	sigmoid(&hidden);

	product(&nn->ho_weights, &hidden, &outputs);
	matrixAdd(&outputs, &nn->o_bias);
	sigmoid(&outputs);

	inputFromArray(target, nn->output, &targets);

	matrixSubtract(&targets, &outputs, &output_errors);

	dsigmoid(&outputs, &gradients);
	matrixMultiply(&gradients, &output_errors);
	multiply(&gradients, nn->learning_rate);

	transpose(&hidden, &hidden_T);
	product(&gradients, &hidden_T, &weight_ho_deltas);

	matrixAdd(&nn->ho_weights, &weight_ho_deltas);
	matrixAdd(&nn->o_bias, &gradients);

	transpose(&nn->ho_weights, &who_t);

	product(&who_t, &output_errors, &hidden_errors);

	dsigmoid(&hidden, &hidden_gradient);
	matrixMultiply(&hidden_gradient, &hidden_errors);
	multiply(&hidden_gradient, nn->learning_rate);

	transpose(&inputs, &inputs_T);
	product(&hidden_gradient, &inputs_T, &weight_ih_deltas);

	matrixAdd(&nn->ih_weights, &weight_ih_deltas);
	//printMatrix(&nn->ih_weights);
	matrixAdd(&nn->h_bias, &hidden_gradient);

	//Dealloc
	deallocMatrix(&inputs);
	deallocMatrix(&hidden);
	deallocMatrix(&outputs);
	deallocMatrix(&targets);
	deallocMatrix(&output_errors);
	deallocMatrix(&gradients);
	deallocMatrix(&hidden_T);
	deallocMatrix(&weight_ho_deltas);
	deallocMatrix(&who_t);
	deallocMatrix(&hidden_errors);
	deallocMatrix(&hidden_gradient);
	deallocMatrix(&inputs_T);
	deallocMatrix(&weight_ih_deltas);

	return 0;
}

int predict(struct NeuralNetwork *nn, float input[], float result[]){
	feedforward(nn, input, result);
	return 0;
}

int randomWithMax(int max){
	return rand() % max;
}