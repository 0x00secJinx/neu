#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Random seed
unsigned long long rdtsc() {
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return ((unsigned long long) hi << 32) | lo;
}

struct Matrix{
	int rows;
	int cols;
	float **data;
} Matrix;

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

int deallocMatrix(struct Matrix *a){
	free(a->data);
	a->data = NULL;
	return 0;
}

int allocMatrix(struct Matrix *matrix){

	// use malloc to init matrix with zeros
	// alloc rows
	matrix->data = malloc(matrix->rows * sizeof(float *));
	if (matrix->data == NULL) {
		return -1;
	}
	//alloc cols
	for(int i = 0; i < matrix->rows; i++){
		matrix->data[i] = malloc(matrix->cols*sizeof(float));
		if (matrix->data[i] == NULL) {
			return -1;
		}
	}

	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = 0.0;
		}
	}
	return 0;
}

int sigmoid(struct Matrix *result){
	//1 / (1 + (exp(-x)));
	for(int i = 0; i < result->rows; i++){
		for(int j = 0; j < result->cols; j++){
			result->data[i][j] = 1 / (1 + exp(-result->data[i][j]));
		}
	}
	return 0;
}

int dsigmoid(struct Matrix *a, struct Matrix *result){
	result->rows = a->rows;
	result->cols = a->cols;
	allocMatrix(result);
	//y * (1-y);
	for(int i = 0; i < result->rows; i++){
		for(int j = 0; j < result->cols; j++){
			result->data[i][j] = a->data[i][j] * (1-a->data[i][j]);
		}
	}
	return 0;
}


float randomwithoutzero(void){
	float randomint = -1.0;
	while(randomint == -1.0){
		randomint = (((double)(rand() % 10)/(double)10)*2-1);
	}
	return randomint;
}

int randomizeMatrix(struct Matrix *matrix){
	//randomize the values of the provided matrix
	srand(rdtsc());
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++) {
			matrix->data[i][j] = randomwithoutzero();
		}
	}
	return 0;
}

int printMatrix(struct Matrix *matrix){
	for(int i = 0; i < matrix->rows; i++){
		printf("[");
		for(int j = 0; j < matrix->cols; j++){
			printf(" %.6f ", matrix->data[i][j]);
		}
		printf("]\n");
	}
	return 0;
}

int add(struct Matrix *matrix, float a){
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = matrix->data[i][j] + a;
		}
	}
	return 0;
}

int multiply(struct Matrix *matrix, float a){
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = matrix->data[i][j] * a;
		}
	}
	return 0;
}

int product(struct Matrix *a, struct Matrix *b, struct Matrix *result){
	float sum;
	if(a->cols != b->rows){
		printf("Could not perform matrix product\n");
		printf("Product Col v. Row Check\n");
j		return -1;
	}
	result->rows = a->rows;
	result->cols = b->cols;
	allocMatrix(result);

	for(int i = 0; i < a->rows; i++){
		for(int j = 0; j < b->cols; j++){
			sum = 0;
			for(int k = 0; k < a->cols; k++){
				sum += a->data[i][k] * b->data[k][j];
			}
			result->data[i][j] = sum;
		}
	}
	return 0;
}

int fromArray(struct Matrix *matrix, float arr[]){
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = arr[j+i*matrix->cols];
		}
	}
	return 0;
}

int transpose(struct Matrix *input, struct Matrix *transposed){
	transposed->rows = input->cols;
	transposed->cols = input->rows;
	allocMatrix(transposed);

	for(int i = 0; i < input->rows; i++){
		for(int j = 0; j < input->cols; j++){
			transposed->data[j][i] = input->data[i][j];
		}
	}
	return 0;
}

int matrixAdd(struct Matrix *a, struct Matrix *b){
	if(a->rows != b->rows){
		printf("matrixAdd: matricies are not same dimensions\n");
		return 0;
	}
	if(a->cols != b->cols){
		printf("matrixAdd: matricies are not same dimensions\n");
		return 0;
	}

	for(int i = 0; i < a->rows; i++){
		for(int j = 0; j < a->cols; j++){
			a->data[i][j] = a->data[i][j] + b->data[i][j];
		}
	}

	return 0;
}

int matrixMultiply(struct Matrix *a, struct Matrix *b){
	if(a->rows != b->rows){
		printf("matrixAdd: matricies are not same dimensions\n");
		return -1;
	}
	if(a->cols != b->cols){
		printf("matrixAdd: matricies are not same dimensions\n");
		return -1;
	}

	for(int i = 0; i < a->rows; i++){
		for(int j = 0; j < a->cols; j++){
			a->data[i][j] = a->data[i][j] * b->data[i][j];
		}
	}

	return 0;
}

int matrixSubtract(struct Matrix *a, struct Matrix *b, struct Matrix *result){

	if(a->rows != b->rows){
		printf("matrixAdd: matricies are not same dimensions\n");
		return -1;
	}
	if(a->cols != b->cols){
		printf("matrixAdd: matricies are not same dimensions\n");
		return -1;
	}

	result->rows = a->rows;
	result->cols = a->cols;
	allocMatrix(result);

	for(int i = 0; i < result->rows; i++){
		for(int j = 0; j < result->cols; j++){
			result->data[i][j] = a->data[i][j] - b->data[i][j];
		}
	}

	return 0;
}

int matrixToArray(struct Matrix *a, float arr[]){
	for(int i = 0; i < a->rows; i++){
		for(int j = 0; j < a->cols; j++){
			arr[j+(i*a->cols)] = a->data[i][j];
		}
	}
	return 0;
}


int inputFromArray(float arr[], int arr_length, struct Matrix *result){

	result->rows = arr_length;
	result->cols = 1;
	allocMatrix(result);

	for(int i = 0; i < result->rows; i++){
		for(int j = 0; j < result->cols; j++){
			result->data[i][j] = arr[j+(i*result->cols)];
		}
	}

	return 0;
}

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

int main(int argc, char *argv[]){
	struct NeuralNetwork nn;
	initNeuralNetwork(&nn, 2, 25, 1, 0.9999);
	int index;
	//Find a way to determine the # of total possible inputs
	float inputs[4][2] = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
	float outputs[4][1] = {{1.0}, {0.0}, {0.0}, {1.0}};
	float result[nn.output];

	for(int i = 0; i < 7500; i++){
		index = randomWithMax(4);
		train(&nn, inputs[index], outputs[index]);
	}

	printf("[0,0]: output should be close to 1\n");
	feedforward(&nn, inputs[0], result);
	for(int i = 0; i < nn.output; i++){
		printf("\t%f\n", result[i]);
	}
	printf("[0,1]: output should be close to 0\n");
	feedforward(&nn, inputs[1], result);
	for(int i = 0; i < nn.output; i++){
		printf("\t%f\n", result[i]);
	}
	printf("[1,0]: output should be close to 0\n");
	feedforward(&nn, inputs[2], result);
	for(int i = 0; i < nn.output; i++){
		printf("\t%f\n", result[i]);
	}
	printf("[1,1]: output should be close to 1\n");
	feedforward(&nn, inputs[3], result);
	for(int i = 0; i < nn.output; i++){
		printf("\t%f\n", result[i]);
	}

}