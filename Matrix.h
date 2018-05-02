#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// A sufficient seed for the random function
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

// Remove the matrix data from the heap
int deallocMatrix(struct Matrix *a){
	for(int i = 0; i < a->rows; i++){
		free(a->data[i]);
	}
	free(a->data);
	a->data = NULL;
	return 0;
}

// Alloc space in heap for matrix data
int allocMatrix(struct Matrix *matrix){

	// use malloc to init matrix with zeros
	// alloc rows
	matrix->data = (float**)malloc(matrix->rows * sizeof(float *));
	if (matrix->data == NULL) {
		return -1;
	}
	//alloc cols
	for(int i = 0; i < matrix->rows; i++){
		matrix->data[i] = (float*)malloc(matrix->cols*sizeof(float));
		if (matrix->data[i] == NULL) {
			return -1;
		}
	}

	// fill matrix with zeros
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = 0.0;
		}
	}
	return 0;
}

int sigmoid(struct Matrix *result){
	// The activation function that will always give a
	// number between 0 and 1
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
	// To avoid zeros in a matrix that will
	// mess with the multiplication, return
	// a number between (-1, 0) and (0, 1)
	// with zero excluded
	float randomint = -1.0;
	while(randomint == -1.0){
		randomint = ((float)(rand() % 10) / (float)11) * 2 - 0.9;
	}
	return randomint;
}

int randomizeMatrix(struct Matrix *matrix){
	//randomize the values of the provided matrix
	srand(rdtsc());
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++) {
			matrix->data[i][j] = randomwithoutzero();
			//matrix->data[i][j] = (float)0.50000;
		}
	}
	return 0;
}

int printMatrix(struct Matrix *matrix){
	// Print the matrix passed, mainly for debugging
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
	// Add 'a' to every element in the matrix
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = matrix->data[i][j] + a;
		}
	}
	return 0;
}

int multiply(struct Matrix *matrix, float a){
	// Multiply 'a' to every element in the matrix
	for(int i = 0; i < matrix->rows; i++){
		for(int j = 0; j < matrix->cols; j++){
			matrix->data[i][j] = matrix->data[i][j] * a;
		}
	}
	return 0;
}

int product(struct Matrix *a, struct Matrix *b, struct Matrix *result){
	// Perform a matrix product on a and b and store in result
	float sum;
	if(a->cols != b->rows){
		printf("Could not perform matrix product\n");
		printf("Product Col v. Row Check\n");
		return -1;
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
